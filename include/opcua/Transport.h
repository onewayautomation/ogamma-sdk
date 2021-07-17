#ifndef owa_transport_h
#define owa_transport_h
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include <string>
#include "boost/any.hpp"
#include <vector>
#include <stdint.h>
#include "opcua/LocalizedText.h"
#include <memory>
#include "opcua/OperationResult.h"

#include "opcua/CallbackFunctionDefs.h"
#include "opcua/ServiceFault.h"
#include "opcua/OpenSecureChannelRequest.h"
#include "opcua/OpenSecureChannelResponse.h"
#include "opcua/FindServersRequest.h"
#include "opcua/FindServersResponse.h"
#include "opcua/GetEndpoints.h"
#include "opcua/CloseSecureChannel.h"
#include "opcua/Session.h"
#include "opcua/Browse.h"
#include "opcua/Attribute.h"
#include "opcua/Subscribe.h"
#include "opcua/Monitor.h"
#include "opcua/Call.h"
#include "opcua/TransportSettings.h"
#include <future>
#include "opcua/LifeTimeWatch.h"
#include "opcua/IpAddressResolver.h"

namespace OWA {
  namespace OpcUa{
    class DataBuffer;
    class Transport: public std::enable_shared_from_this<Transport> {
    public:
      Transport():std::enable_shared_from_this<Transport>() {};
      virtual ~Transport() {};

			virtual void setConfiguration(const TransportSettings& config) = 0;
			virtual TransportSettings getConfiguration() const = 0;

      virtual void setCallback(std::shared_ptr<connectionStateChangeCallback>& f) = 0;

      virtual void setCallback(std::shared_ptr<onResponseReceived<OpenSecureChannelResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<CloseSecureChannelResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<ServiceFaultResponse>>& f) = 0;

      virtual void setCallback(std::shared_ptr<onResponseReceived<FindServersResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<GetEndpointsResponse>>& f) = 0;

      virtual void setCallback(std::shared_ptr<onResponseReceived<CreateSessionResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<ActivateSessionResponse>>& f) = 0;
			virtual void setCallback(std::shared_ptr<onResponseReceived<CloseSessionResponse>>& f) = 0;

      virtual void setCallback(std::shared_ptr<onResponseReceived<BrowseResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<BrowseNextResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<TranslateBrowsePathsToNodeIdsResponse>>& f) = 0;
      
      virtual void setCallback(std::shared_ptr<onResponseReceived<ReadResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<WriteResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<HistoryReadResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<HistoryUpdateResponse>>& f) = 0;
      
      virtual void setCallback(std::shared_ptr<onResponseReceived<CreateSubscriptionResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<ModifySubscriptionResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<DeleteSubscriptionsResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<TransferSubscriptionsResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<SetPublishingModeResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<PublishResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<RepublishResponse>>& f) = 0;
      
      virtual void setCallback(std::shared_ptr<onResponseReceived<CreateMonitoredItemsResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<ModifyMonitoredItemsResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<DeleteMonitoredItemsResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<SetTriggeringResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<SetMonitoringModeResponse>>& f) = 0;

      virtual void setCallback(std::shared_ptr<onResponseReceived<CallResponse>>& f) = 0;

      // This definition is not allowed (template in virtual method):
      //template<typename RequestType, typename ResponseType>
      //virtual void sendRequest(std::shared_ptr<RequestType>& request, onResponseReceived<ResponseType> f) = 0;
      // So method for each type is created:
      virtual bool sendRequest(std::shared_ptr<OpenSecureChannelRequest>& request) = 0;
      virtual bool sendRequest(std::shared_ptr<CloseSecureChannelRequest>& request) = 0;

      virtual bool sendRequest(std::shared_ptr<FindServersRequest>& request) = 0;
      virtual bool sendRequest(std::shared_ptr<GetEndpointsRequest>& request) = 0;

      virtual bool sendRequest(std::shared_ptr<CreateSessionRequest>& request) = 0;
      virtual bool sendRequest(std::shared_ptr<ActivateSessionRequest>& request) = 0;
			virtual bool sendRequest(std::shared_ptr<CloseSessionRequest>& request) = 0;

      virtual bool sendRequest(std::shared_ptr<BrowseRequest>& request) = 0;
      virtual bool sendRequest(std::shared_ptr<BrowseNextRequest>& request) = 0;
      virtual bool sendRequest(std::shared_ptr<TranslateBrowsePathsToNodeIdsRequest>& request) = 0;

      virtual bool sendRequest(std::shared_ptr<ReadRequest>& request) = 0;
      virtual bool sendRequest(std::shared_ptr<WriteRequest>& request) = 0;
      virtual bool sendRequest(std::shared_ptr<HistoryReadRequest>& request) = 0;
      virtual bool sendRequest(std::shared_ptr<HistoryUpdateRequest>& request) = 0;

      virtual bool sendRequest(std::shared_ptr<CreateSubscriptionRequest>& request) = 0;
      virtual bool sendRequest(std::shared_ptr<ModifySubscriptionRequest>& request) = 0;
      virtual bool sendRequest(std::shared_ptr<DeleteSubscriptionsRequest>& request) = 0;
      virtual bool sendRequest(std::shared_ptr<TransferSubscriptionsRequest>& request) = 0;
      virtual bool sendRequest(std::shared_ptr<SetPublishingModeRequest>& request) = 0;
      virtual bool sendRequest(std::shared_ptr<PublishRequest>& request) = 0;
      virtual bool sendRequest(std::shared_ptr<RepublishRequest>& request) = 0;

      virtual bool sendRequest(std::shared_ptr<CreateMonitoredItemsRequest>& request) = 0;
      virtual bool sendRequest(std::shared_ptr<ModifyMonitoredItemsRequest>& request) = 0;
      virtual bool sendRequest(std::shared_ptr<DeleteMonitoredItemsRequest>& request) = 0;
      virtual bool sendRequest(std::shared_ptr<SetMonitoringModeRequest>& request) = 0;
      virtual bool sendRequest(std::shared_ptr<SetTriggeringRequest>& request) = 0;

      virtual bool sendRequest(std::shared_ptr<CallRequest>& request) = 0;

	    virtual OperationResult connect(const ResolveResultType& endpoints, 
        std::weak_ptr<LifeTimeWatch> ltw, 
        uint32_t timeoutInMilliseconds = 5000) = 0;
     
      virtual void disconnect(bool reportChange = true) = 0;

      virtual void listen(const boost::any& context, const std::string& url) = 0;
      virtual void stopListen(const boost::any& context) = 0;

			virtual bool sendChunkIfFull(std::shared_ptr<DataBuffer>& buffer) = 0;
			virtual void checkTotalMessageSize(uint32_t bytesToSend) = 0;
			virtual uint32_t getMaxSendChunkCount() = 0;
      // Added for testing secure channel renewals
      virtual ChannelSecurityToken getSecureChannelToken() = 0;
      
      virtual const std::string& getName() const = 0;
      virtual void setName(const std::string& newName) = 0;

      virtual void setShutdownFlag(bool value) = 0;
      virtual bool getShutdownFlag() = 0;

    protected:

        // To make sure that callbacks to the parent (Connection class) made when the Connection is alive.
        std::weak_ptr<LifeTimeWatch> parentLifeTimeWatch;
    };
  }
}
#endif