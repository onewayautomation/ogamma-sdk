#ifndef owa_opcua_connection_h
#define owa_opcua_connection_h
#include <botan/init.h>
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
		enum State {
			Initial = 0,
			Connecting,
			Connected,
			CouldNotConnect,
			Disconnecting,
			Disconnected
		};
    public:
      /**
      * Connection object with default parameters: protocol opc.tcp with binary encoding and base cryptography class.
      */
      static std::shared_ptr<Connection> create();
      ~Connection();
    protected:
      Connection();

      void init();

      void setReferenceToSelf(std::weak_ptr<Connection> selfReference){};
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
      
    public:
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
      virtual std::future<OperationResult> connect(generalCallback f = std::bind(&Connection::handleConnectDisconnect, std::placeholders::_1));
			
			void initializeConnectionActions();
			
			void setCallbacksFromTransport();
      
      bool isConnected();

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
				std::shared_ptr<NotificationObserver>& notificationCallback,
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
      * Disconnects client connection
      * If a session was created, then it is closed
      * @param deleteSubscriptions is used if a session was created.
      */
      virtual std::future<OperationResult> disconnect(bool deleteSubscriptions, generalCallback f = std::bind(&Connection::handleConnectDisconnect, std::placeholders::_1));

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

      void onStateChangeCallback(const std::string& endpointUrl, ConnectionState state, const OperationResult& result);

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
        if (getRequestContext(response->header.requestHandle, context, true)) {
          if (context->type != RequestType::getTypeId()) {
            // TODO - handle this error case
          }
					std::promise<std::shared_ptr<ResponseType>>* promise = static_cast<std::promise<std::shared_ptr<ResponseType>>*>(context->promise);
					onResponseCallback<RequestType, ResponseType>* callback = static_cast<onResponseCallback<RequestType, ResponseType>*>(context->callbackFunction);
					std::shared_ptr<RequestType>* pointerToRequest = static_cast<std::shared_ptr<RequestType>*>(context->request);

					if (callback != 0 && pointerToRequest != 0) {
						Utils::getThreadPool()->enqueue(std::bind(*callback, *pointerToRequest, response));
					}
					if (promise != 0) {
						promise->set_value(response);
					}
        }
        else {
          //TODO - log
        }
        return true;
      }

    protected:
			void addConnectionAction(Action action, bool toTheFront = false);
			Action getNextConnectionAction(bool removeFromQueue = false);
			void finishConnectionAttempt(State newState, OperationResult result);
			void reportConnectionProgress(OperationResult result);
      State state;
			std::atomic<State> desiredState;

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
      bool removeConnectContext(State newState);
      int32_t getNumberOfPendingRequests();
			void callPublishRequestsAfterCreateSubscriptioin();
			void startConnectionCheck();
			void processPendingRequests(bool disconnecting, StatusCode statusCode);

			template<typename RequestType, typename ResponseType>
			std::future<std::shared_ptr<ResponseType>> processFailedRequest(
				std::shared_ptr<ClientContext>& context, std::shared_ptr<RequestType>& request, std::shared_ptr<ResponseType>& response, StatusCode serviceResult);

			void saveTimer(timer_id id);
			void onTimerCalled(timer_id id);
			void cancelTimers();

      std::shared_ptr<Transport> transport;
      std::shared_ptr<Codec> codec;
      std::shared_ptr<Cryptor> cryptor;

      std::map<RequestId, std::shared_ptr<ClientContext>>  mapIdToPromise;
      ClientConfiguration clientConfiguration;
      std::function<std::string()> passwordCallback;
      X509Certificate serverCertificate;
      bool initialized;
      bool needToGetEndpoints;
      bool needToFindServer;
      SecurityMode activeSecurityMode;
      std::string activeEndpointUrl;

      uint32_t	requestHandle;
			uint32_t	monitoredItemClientHandle;
			uint32_t  subscriptionClientHandle;

      NodeId		sessionAuthenticationToken;

      std::mutex mutextRequestInit;

      std::mutex mutextRequestQueue;
      ByteString clientNonce;
      std::deque <Action> connectionActions;


			std::mutex mutexPublishing;
			std::map < uint32_t, std::shared_ptr<NotificationObserver>> notificaltionCallbacks;
			std::map < CreateSubscriptionRequest*, std::shared_ptr<NotificationObserver>> pendingNotificaltionCallbacks;
			std::atomic<uint32_t> numberOfPendingPublishRequests;
			std::vector <SubscriptionAcknowledgement> acks;

			// After re-connections all subscriptions and monitored items which existed before disconnected are re-created automatically.
			// This can cause change of server side ID for subscriptions and monitored items, so previsously returned to the application ids might become invalid.
			// So the connection maintaines internal maps of originally returned handles to currently up to date ids, so the application can still use originally 
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

			std::mutex mutexTimers;
			std::set<timer_id> timers;
			std::set<timer_id> orphanTimers;

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
			std::shared_ptr<ClientContext> context(new ClientContext(request->header.requestHandle, request->getTypeId(), promise, callback, new std::shared_ptr<RequestType>(request)));
			context->setTimeout(request->header.timeoutHint);
			bool sentOk = false;
      if (addRequestContext(request->header.requestHandle, context))
      {
				try {
					transport->sendRequest(request);
					sentOk = true;
				}
				catch (OperationResult or1) {
					std::shared_ptr<ResponseType> response;
					std::shared_ptr<ClientContext> c;
					if (getRequestContext(request->header.requestHandle, c, true)) {
						return processFailedRequest(c, request, response, or1.code);
					}
				}
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
			response->header.serviceResult = serviceResult;
			response->header.timestamp = DateTime::now();
			std::promise<std::shared_ptr<ResponseType>>* promise = static_cast<std::promise<std::shared_ptr<ResponseType>>*>(context->promise);
			onResponseCallback<RequestType, ResponseType>* callback = static_cast<onResponseCallback<RequestType, ResponseType>*> (context->callbackFunction);
			std::shared_ptr<RequestType>* pointerToRequest = static_cast<std::shared_ptr<RequestType>*>(context->request);

			// Schedule:
			if (callback != 0) {
				Utils::getThreadPool()->enqueue(*callback, *pointerToRequest, response);
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
  }
}

#endif