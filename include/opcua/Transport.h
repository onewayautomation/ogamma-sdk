#ifndef owa_transport_h
#define owa_transport_h

#include <string>
#include "boost/any.hpp"
#include <vector>
#include <stdint.h>
#include "opcua/LocalizedText.h"
#include "opcua/Configurable.h"
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

namespace OWA {
  namespace OpcUa{
    class Transport : public OpcUa::Configurable {
    public:
      virtual ~Transport() {};

      virtual void setCallback(std::shared_ptr<connectionStateChangeCallback>& f) = 0;

      virtual void setCallback(std::shared_ptr<onResponseReceived<OpenSecureChannelResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<CloseSecureChannelResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<ServiceFaultResponse>>& f) = 0;

      virtual void setCallback(std::shared_ptr<onResponseReceived<FindServersResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<GetEndpointsResponse>>& f) = 0;

      virtual void setCallback(std::shared_ptr<onResponseReceived<CreateSessionResponse>>& f) = 0;
      virtual void setCallback(std::shared_ptr<onResponseReceived<ActivateSessionResponse>>& f) = 0;

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
      virtual void sendRequest(std::shared_ptr<OpenSecureChannelRequest>& request) = 0;
      virtual void sendRequest(std::shared_ptr<CloseSecureChannelRequest>& request) = 0;

      virtual void sendRequest(std::shared_ptr<FindServersRequest>& request) = 0;
      virtual void sendRequest(std::shared_ptr<GetEndpointsRequest>& request) = 0;

      virtual void sendRequest(std::shared_ptr<CreateSessionRequest>& request) = 0;
      virtual void sendRequest(std::shared_ptr<ActivateSessionRequest>& request) = 0;

      virtual void sendRequest(std::shared_ptr<BrowseRequest>& request) = 0;
      virtual void sendRequest(std::shared_ptr<BrowseNextRequest>& request) = 0;
      virtual void sendRequest(std::shared_ptr<TranslateBrowsePathsToNodeIdsRequest>& request) = 0;

      virtual void sendRequest(std::shared_ptr<ReadRequest>& request) = 0;
      virtual void sendRequest(std::shared_ptr<WriteRequest>& request) = 0;
      virtual void sendRequest(std::shared_ptr<HistoryReadRequest>& request) = 0;
      virtual void sendRequest(std::shared_ptr<HistoryUpdateRequest>& request) = 0;

      virtual void sendRequest(std::shared_ptr<CreateSubscriptionRequest>& request) = 0;
      virtual void sendRequest(std::shared_ptr<ModifySubscriptionRequest>& request) = 0;
      virtual void sendRequest(std::shared_ptr<DeleteSubscriptionsRequest>& request) = 0;
      virtual void sendRequest(std::shared_ptr<TransferSubscriptionsRequest>& request) = 0;
      virtual void sendRequest(std::shared_ptr<SetPublishingModeRequest>& request) = 0;
      virtual void sendRequest(std::shared_ptr<PublishRequest>& request) = 0;
      virtual void sendRequest(std::shared_ptr<RepublishRequest>& request) = 0;

      virtual void sendRequest(std::shared_ptr<CreateMonitoredItemsRequest>& request) = 0;
      virtual void sendRequest(std::shared_ptr<ModifyMonitoredItemsRequest>& request) = 0;
      virtual void sendRequest(std::shared_ptr<DeleteMonitoredItemsRequest>& request) = 0;
      virtual void sendRequest(std::shared_ptr<SetMonitoringModeRequest>& request) = 0;
      virtual void sendRequest(std::shared_ptr<SetTriggeringRequest>& request) = 0;

      virtual void sendRequest(std::shared_ptr<CallRequest>& request) = 0;

	    virtual void connect(const std::string& endpointUrl, uint32_t timeoutInMilliseconds = 5000) = 0;
      virtual void disconnect() = 0;

      virtual void listen(const boost::any& context, const std::string& url) = 0;
      virtual void stopListen(const boost::any& context) = 0;
    };
  }
}
#endif