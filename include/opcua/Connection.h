#ifndef owa_opcua_connection_h
#define owa_opcua_connection_h
#include "opcua/Utils.h"
#include "opcua/Cryptor.h"
#include "opcua/LocalizedText.h"
#include "opcua/FindServersRequest.h"
#include "opcua/FindServersResponse.h"
#include "opcua/Configurable.h"
#include "opcua/DiagnosticInfo.h"
#include "opcua/OperationResult.h"
#include "opcua/Transport.h"
#include "opcua/Codec.h"
#include "opcua/CallbackFunctionDefs.h"
#include "opcua/ClientConfiguration.h"
#include "opcua/DataChangeNotification.h"
#include "opcua/ClientContext.h"
#include "opcua/Browse.h"

#include <functional>
#include <future>
#include <map>
#include <vector>
#include <stdint.h>
#include <boost/any.hpp>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>
#include <cassert>
#include <set>

namespace OWA {
  namespace OpcUa {
		// This type callback is called whenever connection state is changed:
		typedef std::function <void(const std::string& endpointUrl, ConnectionState state, const OperationResult& result)> StateChangeCallback;

		class Connection : public Configurable {
		protected:
		enum Action {
			Nothing = 0,
			OpenChannel,
			FindServer,
			GetEndpoint,
			CreateSession,
			ActivateSession,
			CreateSubscriptions,
			CreateMonitoredItems,
			Disconnect
		};

    public:
      /**
      * Connection object with default parameters: protocol opc.tcp with binary encoding and base cryptography class.
      */
      static std::shared_ptr<Connection> create();
			static std::shared_ptr<Connection> create(StateChangeCallback& stateChangeCallback);
      ~Connection();

			std::weak_ptr<Connection> getWeakReference ();

    protected:
      Connection(StateChangeCallback& stateChangeCallback);

      void init();

			void setReferenceToSelf(std::weak_ptr<Connection> reference) { selfReference = reference; };
      std::weak_ptr<Connection> selfReference;

      // Default callback handlers, to make callback function argument optional, so callers of Connection can omit it:
      static void handleConnectDisconnect(const OperationResult& result);
      static bool onFindServersResponse(std::shared_ptr<FindServersRequest>&, std::shared_ptr<FindServersResponse>&);
      static bool onGetEndpointsResponse(std::shared_ptr<GetEndpointsRequest>&, std::shared_ptr<GetEndpointsResponse>&);
			
			static bool onReadResponse(std::shared_ptr<ReadRequest>&, std::shared_ptr<ReadResponse>&);
			static bool onWriteResponse(std::shared_ptr<WriteRequest>&, std::shared_ptr<WriteResponse>&) { return true; };
			
			static bool onBrowseResponse(std::shared_ptr<BrowseRequest>&, std::shared_ptr<BrowseResponse>&) { return true; }
			static bool onBrowseNextResponse(std::shared_ptr<BrowseNextRequest>&, std::shared_ptr<BrowseNextResponse>&) { return true; }
			
			static bool onCreateSubscriptionResponse(std::shared_ptr<CreateSubscriptionRequest>&, std::shared_ptr<CreateSubscriptionResponse>&) { return true; }
			static bool onModifySubscriptionResponse(std::shared_ptr<ModifySubscriptionRequest>&, std::shared_ptr<ModifySubscriptionResponse>&) { return true; }
			static bool onDeleteSubscriptionsResponse(std::shared_ptr<DeleteSubscriptionsRequest>&, std::shared_ptr<DeleteSubscriptionsResponse>&) { return true; }
			static bool onSetPublishingModeResponse(std::shared_ptr<SetPublishingModeRequest>&, std::shared_ptr<SetPublishingModeResponse>&) { return true; }
			static bool onTransferSubscriptionsResponse(std::shared_ptr<TransferSubscriptionsRequest>&, std::shared_ptr<TransferSubscriptionsResponse>&) { return true; }
			static bool onPublishResponse(std::shared_ptr<PublishRequest>&, std::shared_ptr<PublishResponse>&) { return true; }
			static bool onRepublishResponse(std::shared_ptr<RepublishRequest>&, std::shared_ptr<RepublishResponse>&) { return true; }

			static bool onCreateMonitoredItemsResponse(std::shared_ptr<CreateMonitoredItemsRequest>&, std::shared_ptr<CreateMonitoredItemsResponse>&) { return true; }
			static bool onModifyMonitoredItemsResponse(std::shared_ptr<ModifyMonitoredItemsRequest>&, std::shared_ptr<ModifyMonitoredItemsResponse>&) { return true; }
			static bool onSetMonitoringModeResponse(std::shared_ptr<SetMonitoringModeRequest>&, std::shared_ptr<SetMonitoringModeResponse>&) { return true; }
			static bool onDeleteMonitoredItemsResponse(std::shared_ptr<DeleteMonitoredItemsRequest>&, std::shared_ptr<DeleteMonitoredItemsResponse>&) { return true; }
			static bool onSetTriggeringResponse(std::shared_ptr<SetTriggeringRequest>&, std::shared_ptr<SetTriggeringResponse>&) { return true; }
			boost::any context;

    public:

			void setContext(boost::any& newContext);
			boost::any getContext();

      // Configuration settings defined what UA Server we want to connect to.
      void setConfiguration(const ClientConfiguration& config);
      // This function will be called to get a password used to encrypt certificate private key.
      void setPasswordCallbackFunction(std::function<std::string()> passwordCallback);

      /**
       * Starts connection process asynchronously. If endpointUrl is empty, uses the one which is defined in the configuration.
       * If configuration was not set at all, then uses default one: opc.tcp://localhost:4840
       * The application acts as a client. Note that this connection does not affect listening to some port as a server.
       * @param createSession - if true, then creates and activates session as part of connection process.
       * @return OperationResult - result of connection operation.
       */
      virtual std::shared_future<OperationResult> connect(generalCallback f = std::bind(&Connection::handleConnectDisconnect, std::placeholders::_1));
			
			/**
			* Disconnects client connection
			* If a session was created, then it is closed
			* @param deleteSubscriptions is used if a session was created.
			*/
			virtual std::shared_future<OperationResult> disconnect(bool reconnect, bool deleteSubscriptions = true, generalCallback f = std::bind(&Connection::handleConnectDisconnect, std::placeholders::_1));

			/* After this call the Connection object does not call any callback functions, so objects using it can be safely deleted.
			*/
			void shutdown();

			void setCallbacksFromTransport();
      
      bool isConnected();

			bool isDisconnected();


      // Indicates that connection process is in progress:
      bool isConnecting();

      // "send" methods for each type of request:
      /**
      * Sends Find Servers request to the server.
      */
      std::future<std::shared_ptr<FindServersResponse>> send(std::shared_ptr<FindServersRequest>& request,
        onResponseCallback<FindServersRequest, FindServersResponse> f = 
        std::bind(&Connection::onFindServersResponse, std::placeholders::_1, std::placeholders::_2));

      std::future<std::shared_ptr<GetEndpointsResponse>> send(std::shared_ptr<GetEndpointsRequest>& request,
        onResponseCallback<GetEndpointsRequest, GetEndpointsResponse> f =
        std::bind(&Connection::onGetEndpointsResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<ReadResponse>> send(std::shared_ptr<ReadRequest>& request,
				onResponseCallback<ReadRequest, ReadResponse> f =
				std::bind(&Connection::onReadResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<WriteResponse>> send(std::shared_ptr<WriteRequest>& request,
				onResponseCallback<WriteRequest, WriteResponse> f =
				std::bind(&Connection::onWriteResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<BrowseResponse>> send(std::shared_ptr<BrowseRequest>& request,
				onResponseCallback<BrowseRequest, BrowseResponse> f =
				std::bind(&Connection::onBrowseResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<BrowseNextResponse>> send(std::shared_ptr<BrowseNextRequest>& request,
				onResponseCallback<BrowseNextRequest, BrowseNextResponse> f =
				std::bind(&Connection::onBrowseNextResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<CreateSubscriptionResponse>> send(std::shared_ptr<CreateSubscriptionRequest>& request,
				NotificationObserver& notificationCallback,
				onResponseCallback<CreateSubscriptionRequest, CreateSubscriptionResponse> f =
				std::bind(&Connection::onCreateSubscriptionResponse, std::placeholders::_1, std::placeholders::_2) );
			
			std::future<std::shared_ptr<ModifySubscriptionResponse>> send(std::shared_ptr<ModifySubscriptionRequest>& request,
				onResponseCallback<ModifySubscriptionRequest, ModifySubscriptionResponse> f =
				std::bind(&Connection::onModifySubscriptionResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<TransferSubscriptionsResponse>> send(std::shared_ptr<TransferSubscriptionsRequest>& request,
				onResponseCallback<TransferSubscriptionsRequest, TransferSubscriptionsResponse> f =
				std::bind(&Connection::onTransferSubscriptionsResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<SetPublishingModeRequest>> send(std::shared_ptr<SetPublishingModeResponse>& request,
				onResponseCallback<SetPublishingModeRequest, SetPublishingModeResponse> f =
				std::bind(&Connection::onSetPublishingModeResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<DeleteSubscriptionsResponse>> send(std::shared_ptr<DeleteSubscriptionsRequest>& request,
				onResponseCallback<DeleteSubscriptionsRequest, DeleteSubscriptionsResponse> f =
				std::bind(&Connection::onDeleteSubscriptionsResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<PublishResponse>> send(std::shared_ptr<PublishRequest>& request,
				onResponseCallback<PublishRequest, PublishResponse> f =
				std::bind(&Connection::onPublishResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<RepublishResponse>> send(std::shared_ptr<RepublishRequest>& request,
				onResponseCallback<RepublishRequest, RepublishResponse> f =
				std::bind(&Connection::onRepublishResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<CreateMonitoredItemsResponse>> send(std::shared_ptr<CreateMonitoredItemsRequest>& request,
				onResponseCallback<CreateMonitoredItemsRequest, CreateMonitoredItemsResponse> f =
				std::bind(&Connection::onCreateMonitoredItemsResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<ModifyMonitoredItemsResponse>> send(std::shared_ptr<ModifyMonitoredItemsRequest>& request,
				onResponseCallback<ModifyMonitoredItemsRequest, ModifyMonitoredItemsResponse> f =
				std::bind(&Connection::onModifyMonitoredItemsResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<DeleteMonitoredItemsResponse>> send(std::shared_ptr<DeleteMonitoredItemsRequest>& request,
				onResponseCallback<DeleteMonitoredItemsRequest, DeleteMonitoredItemsResponse> f =
				std::bind(&Connection::onDeleteMonitoredItemsResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<SetMonitoringModeResponse>> send(std::shared_ptr<SetMonitoringModeRequest>& request,
				onResponseCallback<SetMonitoringModeRequest, SetMonitoringModeResponse> f =
				std::bind(&Connection::onSetMonitoringModeResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<SetTriggeringResponse>> send(std::shared_ptr<SetTriggeringRequest>& request,
				onResponseCallback<SetTriggeringRequest, SetTriggeringResponse> f =
				std::bind(&Connection::onSetTriggeringResponse, std::placeholders::_1, std::placeholders::_2));

      /** 
      * Starts to listen to the port defined in the URL. If URL is not defined, then whatever is defined in configuration. 
      * If configuration was not set, then listens to default port - 4840
      * Note that this method does not affect client connection.
      */
      virtual std::future<OperationResult> listen(const boost::any& context, const std::string& url = "") { return std::promise<OperationResult>().get_future(); }

      /**
      * Stops listening to the port, so all connected clients will be disconnected.
      */
      virtual std::future<OperationResult> stopListen(const boost::any& context){ return std::promise<OperationResult>().get_future(); };

			// this method is called by lower level transport connection when its state changed.
      void onTransportStateChange(const std::string& endpointUrl, ConnectionState state, const OperationResult& result);

      bool onServiceFault(std::shared_ptr<ServiceFaultResponse>& response);
      bool onOpenSecureChannel(std::shared_ptr<OpenSecureChannelResponse>& response);
			bool onFindServers(std::shared_ptr<FindServersResponse>& response);
      bool onGetEndpoints(std::shared_ptr<GetEndpointsResponse>& response);
      bool onCloseSecureChannel(std::shared_ptr<CloseSecureChannelResponse>& response);
      bool onCreateSession(std::shared_ptr<CreateSessionResponse>& response);
      bool onActivateSession(std::shared_ptr<ActivateSessionResponse>& response);
      
			bool onBrowse(std::shared_ptr<BrowseResponse>& response);
      bool onBrowseNext(std::shared_ptr<BrowseNextResponse>& response);
			bool onTranslateNodeIs(std::shared_ptr<TranslateBrowsePathsToNodeIdsResponse>& response);

      bool onRead(std::shared_ptr<ReadResponse>& response);
			bool onWrite(std::shared_ptr<WriteResponse>& response);

			bool onCreateSubscription(std::shared_ptr<CreateSubscriptionResponse>& response);
			bool onModifySubscription(std::shared_ptr<ModifySubscriptionResponse>& response);
			bool onDeleteSubscriptions(std::shared_ptr<DeleteSubscriptionsResponse>& response);
			bool onSetPublishingMode(std::shared_ptr<SetPublishingModeResponse>& response);
			bool onTransferSubscription(std::shared_ptr<TransferSubscriptionsResponse>& response);
			bool onPublish(std::shared_ptr<PublishResponse>& response);
			bool onRepublish(std::shared_ptr<RepublishResponse>& response);

			bool onCreateMonitoredItems(std::shared_ptr<CreateMonitoredItemsResponse>& response);
			bool onModifyMonitoredItems(std::shared_ptr<ModifyMonitoredItemsResponse>& response);
			bool onDeleteMonitoredItems(std::shared_ptr<DeleteMonitoredItemsResponse>& response);
			bool onSetTriggering(std::shared_ptr<SetTriggeringResponse>& response);
			bool onSetMonitoringMode(std::shared_ptr<SetMonitoringModeResponse>& response);

      // This method is called by the transport when it receives a response over network:
      template<typename RequestType, typename ResponseType>
      bool onResponseReceivedCallback(std::shared_ptr<ResponseType>& response) {
        std::shared_ptr<ClientContext> context;
        if (getRequestContext(response->header.requestId, context, true)) {
          if (context->type != RequestType::getTypeId()) {
						throw "Response type does not match with request type!"; // TODO - handle this error case
          }
					std::promise<std::shared_ptr<ResponseType>>* promise = static_cast<std::promise<std::shared_ptr<ResponseType>>*>(context->promise);
					onResponseCallback<RequestType, ResponseType>* callback = static_cast<onResponseCallback<RequestType, ResponseType>*>(context->callbackFunction);
					std::shared_ptr<RequestType>* pointerToRequest = static_cast<std::shared_ptr<RequestType>*>(context->request);

					if (callback != 0 && pointerToRequest != 0)
					{
						std::shared_ptr<Connection> sp = this->selfReference.lock();
						if (sp)
						{
							onResponseCallback<RequestType, ResponseType> cb = *callback;
							std::shared_ptr<RequestType> request = *pointerToRequest;
							Utils::getCallbackThreadPool()->enqueue([sp, cb, request, response]() mutable
							{
								if (!sp->isShutDown)
								{
									cb(request, response);
								}
							});
						}
					}
					if (promise != 0) {
						try
						{
							promise->set_value(response);
						}
						catch (const std::future_error&)
						{
							std::cout << "promise->set_value failed!" << std::endl;
						}
					}
        }
        else {
          //TODO - log
        }
        return true;
      }

			std::string GetName();

    protected:
			void initializeConnectionActions();
			void addConnectionAction(Action action, bool toTheFront = false);
			Action getNextConnectionAction(bool removeFromQueue = false);
			void finishConnectionAttempt(ConnectionState newState, OperationResult result, bool needToScheduleReconnect);
			void scheduleReconnect();
      ConnectionState state;
			ConnectionState desiredState;

      template<typename RequestType, typename ResponseType>
      std::future<std::shared_ptr<ResponseType>> sendRequest(std::shared_ptr<RequestType>& request, onResponseCallback<RequestType, ResponseType> f);

      void initializeRequestHeader(RequestResponseTypeId type, RequestHeader& header);
			void setClientHandles(CreateMonitoredItemsRequest::Ptr& request);

      /**
      * Checks if context with provided exists. If it does exist, returns false. Otherwise adds and returns true
      */
      bool addRequestContext(RequestId id, std::shared_ptr<ClientContext>& cw);
      
      /**
      * Finds request context looking for it by provided id, and modifies provided ContentWrapper for it.
      * @returns true if context was found, otherwise returns false
      * If request Id is not 0, then it is removed from the map. Otherwise it needs to be removed by special method - removeConnectContext();
      */
      bool getRequestContext(RequestId id, std::shared_ptr<ClientContext>& cw, bool deleteFromMap = true);
			bool removeRequestContext(RequestId id);
      bool removeConnectContext(ConnectionState newState);
      int32_t getNumberOfPendingRequests();
			void callPublishRequestsAfterCreateSubscriptioin();
			void startConnectionCheck();
			void processPendingRequests(bool disconnecting, StatusCode statusCode);

			template<typename RequestType, typename ResponseType>
			std::future<std::shared_ptr<ResponseType>> processFailedRequest(
				std::shared_ptr<ClientContext>& context, std::shared_ptr<RequestType>& request, std::shared_ptr<ResponseType>& response, StatusCode serviceResult);

			template<typename RequestType>
			bool addToRequestQueue(std::shared_ptr<RequestType>& request);

			void onTimerCalled(timer_id id);
			void cancelTimers();

      std::shared_ptr<Transport> transport;
      std::shared_ptr<Codec> codec;
      std::shared_ptr<Cryptor> cryptor;

      std::map<RequestId, std::shared_ptr<ClientContext>>  mapIdToPromise;
      ClientConfiguration clientConfiguration;
      std::function<std::string()> passwordCallback;
      X509Certificate serverCertificate;
      SecurityMode activeSecurityMode;
      std::string activeEndpointUrl;

      uint32_t	requestId;
			uint32_t	monitoredItemClientHandle;
			uint32_t  subscriptionClientHandle;

      NodeId		sessionAuthenticationToken;

      std::mutex mutextRequestInit;

      std::recursive_mutex mutextRequestQueue;
      ByteString clientNonce;
      std::deque <Action> connectionActions;

			std::recursive_mutex mutexPublishing;
			std::map < uint32_t, NotificationObserver> notificaltionCallbacks;
			std::map < CreateSubscriptionRequest*, NotificationObserver> pendingNotificaltionCallbacks;
			std::atomic<uint32_t> numberOfPendingPublishRequests;
			std::vector <SubscriptionAcknowledgement> acks;

			// After re-connections all subscriptions and monitored items which existed before disconnected are re-created automatically.
			// This can cause change of server side ID for subscriptions and monitored items, so previously returned to the application ids might become invalid.
			// So the connection maintains internal maps of originally returned handles to currently up to date ids, so the application can still use originally 
			// returned ids.

			std::mutex mutexIdMaps;
			// Maps client handle of monitored items to server assigned handle.
			std::map<uint32_t, uint32_t> mapItemClientIdToServerId;
			std::map<uint32_t, std::map<uint32_t, uint32_t>> mapItemServerIdToClientId;

			// When subscription is created, its copy is saved, associated with client handle.
			std::map<uint32_t, CreateSubscriptionRequest::Ptr> mapSubIdToRequest;

			// Maps client handle of the subscription to the up to date server handle, and vise versa:
			std::map<uint32_t, uint32_t> mapSubClientIdToServerId;
			std::map<uint32_t, uint32_t> mapSubServerIdToClientId;

			std::map <uint32_t, CreateMonitoredItemsRequest::Ptr> mapSubIdToCreateItemsRequest;

			// Store timer's Id and due time.
			std::map<timer_id, std::chrono::time_point<std::chrono::steady_clock>> timers;

			StateChangeCallback stateChangeCallback;

			void scheduleStateChangeCallback(const std::string& endpointUrl, ConnectionState newState, const OperationResult& result);

			ConnectionState stateOfSecureChannel;
			ConnectionState stateOfSession;
			std::atomic<bool> isShutDown;
    };

    // Implementation of the send (called from "send" method):
    template<typename RequestType, typename ResponseType>
    std::future<std::shared_ptr<ResponseType> > OWA::OpcUa::Connection::sendRequest(
      std::shared_ptr<RequestType>& request, 
      onResponseCallback<RequestType, ResponseType> f )
    {
      std::promise<std::shared_ptr<ResponseType>>* promise = new std::promise<std::shared_ptr<ResponseType>>();
      onResponseCallback<RequestType, ResponseType>* callback = new onResponseCallback<RequestType, ResponseType>(f);

			initializeRequestHeader(request->getTypeId(), request->header);
			std::shared_ptr<ClientContext> context(new ClientContext(request->header.requestId, request->getTypeId(), promise, callback, new std::shared_ptr<RequestType>(request)));
			context->setTimeout(request->header.timeoutHint);
			bool sentOk = false;
      if (addRequestContext(request->header.requestId, context))
      {
				try {
					if (transport)
					{
						transport->sendRequest(request);
						sentOk = true;
					}
					else
					{
						throw OperationResult(StatusCode::BadInvalidState, "Attempt to send on deleted transport");
					}
				}
				catch (OperationResult or1) {
					std::shared_ptr<ResponseType> response;
					std::shared_ptr<ClientContext> c;
					// Remove it from the queue and respond:
					if (getRequestContext(request->header.requestId, c, true)) {
						return processFailedRequest(c, request, response, or1.code);
					}
				}
      }
			else
			{
				throw "Failed to add request into the queue";
			}
			if (sentOk) {
				return promise->get_future();
			}
			else {
				// TODO - log internal error
				std::promise<std::shared_ptr<ResponseType>> pr;
				std::shared_ptr<ResponseType> response(new ResponseType());
				response->header.serviceResult = StatusCode::BadInternalError;
				pr.set_value(response);
				return pr.get_future();
			}
    }

		template<typename RequestType, typename ResponseType>
		std::future<std::shared_ptr<ResponseType>> OWA::OpcUa::Connection::processFailedRequest(std::shared_ptr<ClientContext>& context,
			std::shared_ptr<RequestType>& request, std::shared_ptr<ResponseType>& response, StatusCode serviceResult)
		{
			if (response.get() == 0) {
				response.reset(new ResponseType());
			}
			response->header.requestHandle = request->header.requestHandle;
			response->header.requestId = request->header.requestId;
			response->header.serviceResult = serviceResult;
			response->header.timestamp = DateTime::now();
			std::promise<std::shared_ptr<ResponseType>>* promise = static_cast<std::promise<std::shared_ptr<ResponseType>>*>(context->promise);
			onResponseCallback<RequestType, ResponseType>* callback = static_cast<onResponseCallback<RequestType, ResponseType>*> (context->callbackFunction);
			std::shared_ptr<RequestType>* pointerToRequest = static_cast<std::shared_ptr<RequestType>*>(context->request);

			// Schedule callback:
			
			if (callback != 0) {
				// Capture of strong ptr to Connection will make sure that it is not deleted until all callbacks are done:
				std::shared_ptr<Connection> sp = this->selfReference.lock();
				if (sp)
				{
					onResponseCallback<RequestType, ResponseType> cb = *callback;
					Utils::getCallbackThreadPool()->enqueue([sp, cb, request, response]() mutable
					{
						if (!sp->isShutDown)
						{
							cb(request, response);
						}
					});
				}
			}
			// Set result in the original future:
			if (promise != 0) {
				promise->set_value(response);
			}

			// Create new future to return, as the original future cannot be got more than once.
			// TODO - might need refactoring to use shared future.
			std::promise<std::shared_ptr<ResponseType>> pr;
			pr.set_value(response);
			return pr.get_future();
			//return promise->get_future();
		}

		template<typename RequestType>
		bool OWA::OpcUa::Connection::addToRequestQueue(std::shared_ptr<RequestType>& request)
		{
			std::shared_ptr<ClientContext> context(
				new ClientContext(request->header.requestId, request->getTypeId(), (void*)0, (void*)0, new std::shared_ptr<RequestType>(request)));
			context->setTimeout(request->header.timeoutHint);
			return addRequestContext(request->header.requestId, context);
		}
  }
}

#endif