#ifndef owa_opcua_connection_h
#define owa_opcua_connection_h
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include "opcua/Utils.h"
#include "opcua/Cryptor.h"
#include "opcua/LocalizedText.h"
#include "opcua/FindServersRequest.h"
#include "opcua/FindServersResponse.h"
#include "opcua/DiagnosticInfo.h"
#include "opcua/OperationResult.h"
#include "opcua/Transport.h" 
#include "opcua/Codec.h"
#include "opcua/CallbackFunctionDefs.h"
#include "opcua/ClientConfiguration.h"
#include "opcua/DataChangeNotification.h"
#include "opcua/ClientContext.h"
#include "opcua/Browse.h"
#include "opcua/Attribute.h"

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
#include <atomic>

#include "opcua/OpcUaCertificateStore.h"
#include "opcua/StructuredTypeDefinition.h"

#ifdef WIN32
#pragma comment(lib, "OpcUaSdk.lib")
#pragma comment(lib, "botan.lib")
#endif

namespace OWA {
  namespace OpcUa {
		// This type callback is called whenever connection state is changed:
		typedef std::function <void(std::weak_ptr<Connection>, ConnectionState state, const OperationResult& result)> StateChangeCallback;
		
		// Function is called when namespace manager is updated. This happens either after CreateSession, 
		// or after getting complex data type value in the PublishResponse data change notification with unknown data type 
		// and getting additional type information form the server.
		typedef std::function <void(std::shared_ptr < NamespaceManager>)> NamespaceManagerUpdatedCallback;

		class Connection: public std::enable_shared_from_this<Connection> {
		protected:
		enum struct Action:uint8_t {
			Nothing = 0,
			ResolveFindServersHost,
			OpenChannel,
			FindServer,
			ResolveGetEndpointsHost,
			GetEndpoint,
			ResolveCreateSessionHost,
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
			static std::shared_ptr<Connection> create(const ClientConfiguration& config);
			static std::shared_ptr<Connection> create(const std::string& endpointUrl, bool createSession = false);
			static std::shared_ptr<Connection> create(StateChangeCallback& stateChangeCallback);
			static std::shared_ptr<Connection> create(const ClientConfiguration& config, StateChangeCallback stateChangeCallback);
      virtual ~Connection();

    public:

			void setContext(boost::any& newContext);
			boost::any getContext();

      // Configuration settings defined what UA Server we want to connect to.
      void setConfiguration(const ClientConfiguration& config);
			ClientConfiguration getConfiguration();

			void setCertificateStore(std::shared_ptr<OpcUaCertificateStore> store);

			std::shared_ptr<OpcUaCertificateStore> getCertificateStore();

      /**
       * Starts connection process asynchronously. 
       * @param f - optional, function, which will be called when connection process is finished.
       * @return OperationResult - shared future with result of connection operation.
       * If current state is:
       *  - Disconnected: Normal, most expected case. Starts new connection transaction.
       *  - Disconnecting: Not expected, completes immediately with error.
       *  - Connecting: acceptable, returns existing shared future, so all threads waiting for connection to finish will get the same result.
       *  - Connected: acceptable, completes immediately with warning that already connected.
       */
      virtual std::shared_future<OperationResult> connect(generalCallback f = std::bind(&Connection::handleConnectDisconnect, std::placeholders::_1, std::placeholders::_2));
			
			/**
			* Disconnects client connection
			* If a session was created, then it is closed
      * If a secure channel is created, it is closed.
      * @param reconnect - if true, after disconnection it will reconnect again.
			* @param deleteSubscriptions is used if a session was created.
      * If current state is:
      *   - Disconnected: completes immediately with warning that already disconnected.
      *   - Disconnecting: returns existing (started with previous disconnect call) shared future.
      *   - Connected: Normal, most expected case. Starts new disconnection transaction.
      *   - Connecting: Completes immediately with error. Pending connection must be first finished.
			*/
			virtual std::shared_future<OperationResult> disconnect(bool reconnect = false, bool deleteSubscriptions = true, generalCallback f = std::bind(&Connection::handleConnectDisconnect, std::placeholders::_1, std::placeholders::_2));

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

			std::future<std::shared_ptr<HistoryReadResponse>> send(std::shared_ptr<HistoryReadRequest>& request,
				onResponseCallback<HistoryReadRequest, HistoryReadResponse> f =
				std::bind(&Connection::onHistoryReadResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<HistoryUpdateResponse>> send(std::shared_ptr<HistoryUpdateRequest>& request,
				onResponseCallback<HistoryUpdateRequest, HistoryUpdateResponse> f =
				std::bind(&Connection::onHistoryUpdateResponse, std::placeholders::_1, std::placeholders::_2));


			std::future<std::shared_ptr<WriteResponse>> send(std::shared_ptr<WriteRequest>& request,
				onResponseCallback<WriteRequest, WriteResponse> f =
				std::bind(&Connection::onWriteResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<CallResponse>> send(std::shared_ptr<CallRequest>& request,
				onResponseCallback<CallRequest, CallResponse> f =
				std::bind(&Connection::onCallResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<BrowseResponse>> send(std::shared_ptr<BrowseRequest>& request,
				onResponseCallback<BrowseRequest, BrowseResponse> f =
				std::bind(&Connection::onBrowseResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<BrowseNextResponse>> send(std::shared_ptr<BrowseNextRequest>& request,
				onResponseCallback<BrowseNextRequest, BrowseNextResponse> f =
				std::bind(&Connection::onBrowseNextResponse, std::placeholders::_1, std::placeholders::_2));

			std::future<std::shared_ptr<CreateSubscriptionResponse>> send(std::shared_ptr<CreateSubscriptionRequest>& request,
				NotificationObserver& notificationCallback, bool forceCreate,
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

			std::future<std::shared_ptr<CreateMonitoredItemsResponse>> send(std::shared_ptr<CreateMonitoredItemsRequest>& request, bool forceCreate,
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
			virtual std::future<OperationResult> listen(const boost::any& context, const std::string& url = "") {
				(void)url;
				(void)context;
				return std::promise<OperationResult>().get_future();
			}

      /**
      * Stops listening to the port, so all connected clients will be disconnected.
      */
			virtual std::future<OperationResult> stopListen(const boost::any& context) { 
				(void)context; 
				return std::promise<OperationResult>().get_future(); 
			};

			std::string GetName() const;

			// Return server certificate (if there is any) which was used for the last connection attempt. 
			// Can be used to retrieve the server side certificate after connection failure caused by invalid/not trusted server certificate, to display it to the user.
			X509Certificate getServerCertificate();

			/* Return server instance certificate, and parent certificates (if they were returned by the server in GetEndpoints response). */
			std::vector<X509Certificate> getServerCertificates();

      /* Server certificate as is returned by the server (can be chained or not */
      ByteString getDerEncodedServerCertificate();

			/* Used in unit tests:*/ 
      ChannelSecurityToken getSecureChannelToken();

			void SetStateChangeCallback(StateChangeCallback scc);

			void SetNamespaceManagerUpdatedCallback(NamespaceManagerUpdatedCallback cb);

			std::shared_ptr<NamespaceManager> getNamespaceManager();
			void setNamespaceManager(std::shared_ptr<NamespaceManager> nsManager);
			
			std::string getEndpointUrl();
			
			// Internal debug code, used for troubleshooting.
			int debugCode;

	protected:

			// this method is called by lower level transport connection when its state changed.
      void onTransportStateChange(const std::string& endpointUrl, ConnectionState state, const OperationResult& result);

      bool onServiceFault(std::shared_ptr<ServiceFaultResponse>& response);
      bool onOpenSecureChannel(std::shared_ptr<OpenSecureChannelResponse>& response);
			bool onFindServers(std::shared_ptr<FindServersResponse>& response);
      bool onGetEndpoints(std::shared_ptr<GetEndpointsResponse>& response);
      bool onCloseSecureChannel(std::shared_ptr<CloseSecureChannelResponse>& response);
      bool onCreateSession(std::shared_ptr<CreateSessionResponse>& response);
      bool onActivateSession(std::shared_ptr<ActivateSessionResponse>& response);

			bool onGetServerInfoComplete(OperationResult result);

			bool onCloseSession(std::shared_ptr<CloseSessionResponse>& response);
      
			bool onBrowse(std::shared_ptr<BrowseResponse>& response);
      bool onBrowseNext(std::shared_ptr<BrowseNextResponse>& response);
			bool onTranslateNodeIs(std::shared_ptr<TranslateBrowsePathsToNodeIdsResponse>& response);

      bool onRead(std::shared_ptr<ReadResponse>& response);
			bool onWrite(std::shared_ptr<WriteResponse>& response);

			bool onHistoryRead(std::shared_ptr<HistoryReadResponse>& response);
			bool onHistoryUpdate(std::shared_ptr<HistoryUpdateResponse>& response);

			bool onCall(std::shared_ptr<CallResponse>& response);

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
      bool onResponseReceivedCallback(std::shared_ptr<ResponseType>& response) 
      {
				responseCounter++;
        std::string name = Utils::getName(RequestType::getTypeId());
        std::weak_ptr<LifeTimeWatch> wp = lifeTimeWatch;
				Utils::getCallbackThreadPool()->enqueue([wp, this, response, name]() mutable
				{
					auto sp = wp.lock();
					if (sp)
					{
						std::shared_ptr<ClientContext> context;
						if (this->getRequestContext(response->header.requestId, context, true))
						{
							if (context->type != RequestType::getTypeId())
							{
								spdlog::error("Response type {0} does not match with request type {1} of the context", (int)RequestType::getTypeId(), (int)context->type);
								// TODO - handle this error case. Possible option is disconnect.
							}
							else
							{
								std::promise<std::shared_ptr<ResponseType>>* promise = static_cast<std::promise<std::shared_ptr<ResponseType>>*>(context->promise);
								onResponseCallback<RequestType, ResponseType>* callback = static_cast<onResponseCallback<RequestType, ResponseType>*>(context->callbackFunction);
								std::shared_ptr<RequestType>* pointerToRequest = static_cast<std::shared_ptr<RequestType>*>(context->request);

								if (Utils::isGood(response->header.serviceResult))
									lastCommunicationTime.now();

								std::shared_ptr<RequestType> request;
								{
									if (pointerToRequest != 0)
										request = *pointerToRequest;
								}
								if (callback != 0 && request)
								{
									try 
									{
										auto spToThis = this->shared_from_this();
										if (spToThis) // Keep the connection alive until callback is completed.
										{
											onResponseCallback<RequestType, ResponseType> cb = *callback;
											cb(request, response);
										}
									}
									catch (const std::runtime_error&)
									{
										spdlog::error("Connection: Failed to perform callback at {}", __LINE__);
									}
								}
								if (promise != 0) {
									try
									{
										promise->set_value(response);
									}
									catch (const std::future_error&)
									{
										spdlog::critical("promise->set_value failed in {0}/{1}", __FILE__, __LINE__);
									}
								}
							}
						}
						else
						{
							spdlog::warn("Secure Channel {}: Received {} response with id = {}, but failed to find pending request for it.",
								this->getSecureChannelToken().SecureChannelId, name, response->header.requestId);
						}
						sp->onCallbackComplete();
					}
				});
				return true;
      }
			   
      Connection(const ClientConfiguration& config, StateChangeCallback& stateChangeCallback);

      void init();

			void setCallbacksFromTransport();
			
			ByteString getMyCertificate(bool fullChain = true);

      // Default callback handlers, to make callback function argument optional, so callers of Connection can omit it:
      static void handleConnectDisconnect(std::weak_ptr<Connection>, const OperationResult& result);
      static bool onFindServersResponse(std::shared_ptr<FindServersRequest>&, std::shared_ptr<FindServersResponse>&);
      static bool onGetEndpointsResponse(std::shared_ptr<GetEndpointsRequest>&, std::shared_ptr<GetEndpointsResponse>&);

      static bool onReadResponse(std::shared_ptr<ReadRequest>&, std::shared_ptr<ReadResponse>&);
      static bool onWriteResponse(std::shared_ptr<WriteRequest>&, std::shared_ptr<WriteResponse>&) { return true; };
      static bool onHistoryReadResponse(std::shared_ptr<HistoryReadRequest>&, std::shared_ptr<HistoryReadResponse>&) { return true; };
      static bool onHistoryUpdateResponse(std::shared_ptr<HistoryUpdateRequest>&, std::shared_ptr<HistoryUpdateResponse>&) { return true; };

      static bool onCallResponse(std::shared_ptr<CallRequest>&, std::shared_ptr<CallResponse>&) { return true; };

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
      boost::any applicationContext;

      //// Returns true if this was last callback, and it resulted completion of the disconnection.
      //bool onCallbackCompleted(const std::string& name = "");

      //void onCallbackScheduled(const std::string& name = "");

      std::map<std::string, uint32_t> pendingCallbacks;

      ConnectionState getSessionState();

      //// If there are no pending callbacks other then reconnect, and there is a pending disconnection transaction, will complete that disconnect transaction.
      //bool tryToCompleteDisconnection(const OperationResult& result = OperationResult(StatusCode::Good, "Disconnected"));

      std::shared_future<OperationResult> completeConnectionImpl(
				ConnectionState stateToReport,
        const OperationResult operationResultToReport);

      std::shared_future<OperationResult> completeDisconnectionImpl(
				const OperationResult operationResultToReport = OperationResult(StatusCode::Good));

      std::shared_ptr<CreateSessionRequest> createCreateSessionReqeust();

			OperationResult initializeConnectionActions();
			void addConnectionAction(Action action, bool toTheFront = false);
			Action getNextConnectionAction(bool removeFromQueue = false);
			
			std::shared_future<OperationResult> finishConnectionAttempt(const ConnectionState newState, const OperationResult& result);
			std::shared_future<OperationResult> finishConnectionAttempt(std::unique_lock<std::recursive_mutex>& stateLock, const ConnectionState newState, const OperationResult& result);

			std::shared_future<OperationResult> finishDisconnectionAttempt(const OperationResult& result = OperationResult(StatusCode::Good));
			std::shared_future<OperationResult> finishDisconnectionAttempt(std::unique_lock<std::recursive_mutex>& stateLock, const OperationResult& result = OperationResult(StatusCode::Good));

			bool scheduleReconnect();
			void scheduleSecureChannelRenewal();
      ConnectionState state;
			ConnectionState desiredState;

      template<typename RequestType, typename ResponseType>
      std::future<std::shared_ptr<ResponseType>> sendRequest(std::shared_ptr<RequestType>& request, onResponseCallback<RequestType, ResponseType> f);

      void initializeRequestHeader(RequestResponseTypeId type, RequestHeader& header);
			void setClientHandles(CreateMonitoredItemsRequest::Ptr& request);

      bool handlePublishResponse(std::shared_ptr<PublishRequest>& request, std::shared_ptr<PublishResponse>& response);
      /**
      * Checks if context with provided exists. If it does exist, returns false. Otherwise adds and returns true
      */
      bool addRequestContext(RequestId id, std::shared_ptr<ClientContext>& cw);
      
      /**
      * Finds request context looking for it by provided id, and modifies provided ContentWrapper for it.
      * @returns true if context was found, otherwise returns false
      */
      bool getRequestContext(RequestId id, std::shared_ptr<ClientContext>& cw, bool deleteFromMap = true);
			bool removeRequestContext(RequestId id);
      int32_t getNumberOfPendingRequests();
			void callPublishRequestsAfterCreateSubscription(uint32_t timeoutValue);
			void startConnectionCheck();

			void scheduleProcessingOfPendingRequests();
			int numberOfScheduledPendingRequestProcessTimers;

			void processPendingRequests(bool disconnecting, StatusCode statusCode, bool doNotCheckChangingFlag = false);

			template<typename RequestType>
			bool addToRequestQueue(std::shared_ptr<RequestType>& request);

			void onTimerCalled(timer_id id);
			void cancelTimers(std::unique_lock<std::recursive_mutex>& stateLock);

			OperationResult validateRemoteCertificate(const std::string validationEndpointUrl = "");

			OWA::OpcUa::OperationResult readPrimitiveSubtypes(std::shared_ptr<ClientConfiguration> clientConfig, std::shared_ptr<NamespaceManager> nsm, 
				std::vector<NodeId> nodesToBrowse, std::vector<QualifiedName> baseTypeName, int level);

			OperationResult readEnumerations(std::shared_ptr<ClientConfiguration> clientConfig, std::shared_ptr<NamespaceManager> nsm, std::vector<NodeId> nodesToBrowse, int level);

			OperationResult readDataTypeDefinitions(std::shared_ptr<ClientConfiguration> clientConfig, std::shared_ptr<NamespaceManager> nsm, std::vector<NodeId> nodesToBrowse, int level);

			std::shared_ptr<Connection> getSelfReference();

      std::shared_ptr<Transport> transport;
      std::shared_ptr<Codec> codec;
      std::shared_ptr<Cryptor> cryptor;

      std::map<RequestId, std::shared_ptr<ClientContext>>  mapIdToPromise;
      ClientConfiguration clientConfiguration;
      
			/* End (leaf) certificate for the server. */
      X509Certificate serverCertificate;

			/* One or more certificates, received via GetEndpoints call from the server, as it was returned. 
			In most cases server return only leaf certificate, but it they return whole chain, then users can view it and trust one at selected level from GUI.	*/
			std::vector<OWA::OpcUa::X509Certificate> serverCertificates;

			/* server certificate as received from the server, not decoded. Can be chained or not */
			ByteString encodedServerCertificate;

			ApplicationDescription remoteAppDescription;
			OpcUaCertificateStore::ValidationRules validationRules;

      SecurityMode activeSecurityMode;
			std::string activeEndpointUrl;

			ResolveResultType endpointForFindServers;
			ResolveResultType endpointForGetEndpoints;
			ResolveResultType endpointForCreateSession;

			std::shared_ptr<IpAddressResolver> resolver;
			// std::string activeEndpointUrl;

			// Remember host name/IP address which was used for successful connections, and use them later if connection does not work.
			uint32_t connectionAttempt;
			std::string findServersHost;
			std::string getEndpointsHost;

      uint32_t	requestId;
			uint32_t	monitoredItemClientHandle;
			uint32_t  subscriptionClientHandle; 

      NodeId		sessionAuthenticationToken;

      
			// Mutex to protect current state of the Connection.
			// It can be unlocked for duration of calls to the Transport.
			// To keep the state un-changed during those unlocks, variable 'changingState' is used in combination with the 'stateConditionVariable'.
			std::recursive_mutex mutexState;
      int timeoutMutexState;
			bool changingState;
			std::condition_variable_any stateConditionVariable;

			std::condition_variable_any timerConditionVariable;

      ByteString clientNonce;
      std::deque <Action> connectionActions;
			std::shared_ptr<ConnectionDisconnectContext> currentTransaction;

			std::condition_variable_any publishingConditionVariable;
      bool publishingBusy;
      int timeoutMutexPublishing;

			std::map < uint32_t, NotificationObserver> notificaltionCallbacks;
			std::map < CreateSubscriptionRequest*, NotificationObserver> pendingNotificaltionCallbacks;
			uint32_t numberOfPendingPublishRequests;
			std::vector <SubscriptionAcknowledgement> acks;

			// Flag to indicate that at least for some monitored items Publish response with data changes is received after connection.
			// Initial value is false.
			// Used to figure out, if we need to make a data change notification callback after disconnection: called if this flag is true.
			std::atomic<bool> dataChangesReceived;

			// After re-connections all subscriptions and monitored items which existed before disconnected are re-created automatically.
			// This can cause change of server side ID for subscriptions and monitored items, so previously returned to the application ids might become invalid.
			// So the connection maintains internal maps of originally returned handles to currently up to date ids, so the application can still use originally 
			// returned ids.

			// Maps client handle of monitored items to server assigned handle.
			std::map<uint32_t, uint32_t> mapItemClientIdToServerId;
			std::map<uint32_t, std::map<uint32_t, uint32_t>> mapItemServerIdToClientId;

			// When subscription is created, its copy is saved, associated with client handle.
			std::map<uint32_t, CreateSubscriptionRequest::Ptr> mapSubIdToRequest;

			// Maps client handle of the subscription to the up to date server handle, and vise versa:
			std::map<uint32_t, uint32_t> mapSubClientIdToServerId;
			std::map<uint32_t, uint32_t> mapSubServerIdToClientId;

			std::map <uint32_t, CreateMonitoredItemsRequest::Ptr> mapSubIdToCreateItemsRequest;
      
			// Counters to track numbers of create subscription and create monitored items responses received during connection process:
			std::atomic<size_t> createMonitoredItemsRequestsToSendAtConnection;
      std::atomic<size_t> createSubscriptionRequestsToSendAtConnection;

			// Store timer's Id, name and due time.
			std::map<timer_id, std::pair<std::string, std::chrono::time_point<std::chrono::steady_clock>>> pendingTimers;

			StateChangeCallback stateChangeCallback;
			NamespaceManagerUpdatedCallback namespaceManagerUpdatedCallback;

			void scheduleStateChangeCallback(const std::string& endpointUrl, ConnectionState newState, const OperationResult& result);

			void callStateChangeCallback(std::unique_lock<std::recursive_mutex>& stateLock, ConnectionState newState, const OperationResult& result);
			void callStateChangeCallback(const std::string& endpointUrl, ConnectionState newState, const OperationResult& result);

			ConnectionState stateOfSecureChannel;
			ConnectionState stateOfSession;

			// isReconnecting - set to true if rec-connecting with different security mode or to different UEL during initial connection, 
			// or method "disconnect" is called with intention to re-connect after disconnection.
      bool isReconnecting;

      uint32_t numberOfScheduledCallbacks;
      std::atomic<bool> shuttingDown;
			std::atomic<bool> reconnectionScheduled;

			// Variable to track number of scheduled Open Secure Channel requests (renewals).
			uint16_t numberOfScheduledChannelRenewals;

			std::shared_ptr<OpcUaCertificateStore> certificateStore;

			// Method to collect information about complex data types.
			OperationResult startGettingServerInfo();

			void onHostNameResolved(const ResolveResultType& result, Action action, std::weak_ptr<LifeTimeWatch> ltw);

			OperationResult moveToNextConnectionAction(std::unique_lock<std::recursive_mutex>& stateLock);

			OperationResult connectTransport(Action action, const ClientConfiguration& clientConfig);
			
			// Method to quickly disconnect transport. Used during connecting to reconnect with different security mode, or to different endpoint URL
			// If secure channel is open, will send close secure channel request. 
			void disconnectTransport(bool reconnecting, std::unique_lock<std::recursive_mutex>& stateLock, bool reportChange, 
				const OperationResult& resultToReport = OperationResult(StatusCode::Good));

			std::shared_ptr<NamespaceManager> namespaceManager;

			std::weak_ptr<Connection> selfReference;

			// This object instance is created when new connection transaction starts, and exists until it is disconnected.
			// When callbacks are scheduled, they should get weak pointer from it,
			// and when callback is received, should get strong pointer form it, and continue only if strong pointer is valid.
			// This object is also used to make sure that no callbacks to the application layer made after disconnection.
			std::shared_ptr<LifeTimeWatch> lifeTimeWatch; // Own watch
			std::shared_ptr<LifeTimeWatch> transportLifeTimeWatch;

			std::shared_ptr<LifeTimeWatch> timerLifetimeWatch;

			OperationResult lastReportedResult;
			ConnectionState lastReportedState;

			DateTime lastCommunicationTime;
			ConnectionState lastMonitoredState;
			std::atomic<uint64_t> responseCounter;

			// Id used to identify the object instance. 
			std::atomic <uint32_t> id;
			static std::atomic<uint32_t> lastId;
			std::string name;
			std::atomic<bool> statusMonitorIsInProgress;

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
			std::shared_ptr<ClientContext> sendContext(new ClientContext(request->header.requestId, request->getTypeId(), promise, callback, new std::shared_ptr<RequestType>(request)));
			sendContext->setTimeout(request->header.timeoutHint);
			bool sentOk = false;

			// To make sure that the future has a valid state before other thread tries to call get on it, acquire it before sending of the request:
			auto futureToReturn = promise->get_future();
      if (addRequestContext(request->header.requestId, sendContext))
      {
				try {
					if (transport)
					{
						sentOk = transport->sendRequest(request);
						if (!sentOk)
							throw OperationResult(request->header.sendResult, "Failed to send request.");
					}
					else
					{
						spdlog::critical("Attemp to send on deleted transport detected at {}/{}", __FILE__, __LINE__);
						throw OperationResult(StatusCode::BadInvalidState, "Attempt to send on deleted transport");
					}
				}
				catch (OperationResult or1) {
					std::shared_ptr<ServiceFaultResponse> response(new ServiceFaultResponse());
          response->header.requestHandle = request->header.requestHandle;
          response->header.requestId = request->header.requestId;
          response->header.serviceResult = or1.code;
          response->header.timestamp.now();
					onServiceFault(response);
				}
      }
			else
			{
        std::stringstream s;
        s << "Failed to add request into the queue at " << __FILE__ << "/" << __LINE__;
        spdlog::critical("[{}]: {}", GetName(), s.str());
        throw CriticalErrorException(s.str());
			}
			if (sentOk) {
				return std::move(futureToReturn);
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

		template<typename RequestType>
		bool OWA::OpcUa::Connection::addToRequestQueue(std::shared_ptr<RequestType>& request)
		{
			std::shared_ptr<ClientContext> ctx(
				new ClientContext(request->header.requestId, request->getTypeId(), (void*)0, (void*)0, new std::shared_ptr<RequestType>(request)));
			ctx->setTimeout(request->header.timeoutHint);
			return addRequestContext(request->header.requestId, ctx);
		}
  }
}

#endif