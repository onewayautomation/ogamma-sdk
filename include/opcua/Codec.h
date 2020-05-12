#pragma once
#include "opcua/Cryptor.h"
#include "opcua/MessageHeader.h"
#include <boost/asio/buffer.hpp>
#include "opcua/ServiceFault.h"
#include "opcua/OpenSecureChannelRequest.h"
#include "opcua/OpenSecureChannelResponse.h"
#include "opcua/FindServersRequest.h"
#include "opcua/FindServersResponse.h"
#include "opcua/GetEndpoints.h"
#include "opcua/CloseSecureChannel.h"

#include "opcua/ByteString.h"
#include "opcua/EncodedExtensionObject.h"
#include "opcua/Guid.h"
#include "opcua/LocalizedText.h"
#include "opcua/SymmetricCryptoContext.h"
#include "opcua/DataBuffer.h"
#include "opcua/Session.h"
#include "opcua/Browse.h"
#include "opcua/Attribute.h"
#include "opcua/Subscribe.h"
#include "opcua/Monitor.h"
#include "opcua/Call.h"
#include <memory>
#include <stdint.h>

namespace OWA {
  namespace OpcUa {
    class Codec {
    public:
      virtual void setSequenceNumber(uint32_t newValue = 0) = 0;
      virtual void encode(DataBufferPtr& buffer, HelloMessage& body) = 0;

      virtual void decode(DataBufferPtr&  buffer, std::shared_ptr<MessageHeader>& result) = 0;
      virtual void decode(DataBufferPtr&  buffer, std::shared_ptr<HelloMessage>& result) = 0;
      virtual void decode(DataBufferPtr&  buffer, std::shared_ptr<AchnowledgeMessage>& result) = 0;
      virtual void decode(DataBufferPtr&  buffer, std::shared_ptr<ErrorMessage>& result) = 0;
      virtual void decode(TcpReadContextPtr&, std::shared_ptr<AssymmetricAlgortrithmSecurityHeader>& value) = 0;
      virtual void decode(TcpReadContextPtr&, std::shared_ptr<SymmetricAlgorithmSecurityHeader>& value) = 0;
      virtual void decode(TcpReadContextPtr&, std::shared_ptr<SymmetricCryptoContext>& context, std::shared_ptr<SymmetricAlgorithmSecurityHeader>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, OpenSecureChannelRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, OpenSecureChannelResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, OpenSecureChannelRequest& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<OpenSecureChannelResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, FindServersRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, FindServersResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<FindServersRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<FindServersResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, GetEndpointsRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, GetEndpointsResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<GetEndpointsRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<GetEndpointsResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, CloseSecureChannelRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, CloseSecureChannelResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CloseSecureChannelRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CloseSecureChannelResponse>& value) = 0;

      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<ServiceFaultResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, CreateSessionRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, CreateSessionResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CreateSessionRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CreateSessionResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, ActivateSessionRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, ActivateSessionResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<ActivateSessionRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<ActivateSessionResponse>& value) = 0;

			virtual void encode(DataBufferPtr& buffer, CloseSessionRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
			virtual void encode(DataBufferPtr& buffer, CloseSessionResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
			virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CloseSessionRequest>& value) = 0;
			virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CloseSessionResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, BrowseRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, BrowseResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<BrowseRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<BrowseResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, BrowseNextRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, BrowseNextResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<BrowseNextRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<BrowseNextResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, TranslateBrowsePathsToNodeIdsRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, TranslateBrowsePathsToNodeIdsResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<TranslateBrowsePathsToNodeIdsRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<TranslateBrowsePathsToNodeIdsResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, ReadRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, ReadResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<ReadRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<ReadResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, WriteRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, WriteResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<WriteRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<WriteResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, HistoryReadRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, HistoryReadResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<HistoryReadRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<HistoryReadResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, HistoryUpdateRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, HistoryUpdateResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<HistoryUpdateRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<HistoryUpdateResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, CreateSubscriptionRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, CreateSubscriptionResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CreateSubscriptionRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CreateSubscriptionResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, ModifySubscriptionRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, ModifySubscriptionResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<ModifySubscriptionRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<ModifySubscriptionResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, DeleteSubscriptionsRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, DeleteSubscriptionsResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<DeleteSubscriptionsRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<DeleteSubscriptionsResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, TransferSubscriptionsRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, TransferSubscriptionsResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<TransferSubscriptionsRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<TransferSubscriptionsResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, SetPublishingModeRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, SetPublishingModeResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<SetPublishingModeRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<SetPublishingModeResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, PublishRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, PublishResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<PublishRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<PublishResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, RepublishRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, RepublishResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<RepublishRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<RepublishResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, CreateMonitoredItemsRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, CreateMonitoredItemsResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CreateMonitoredItemsRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CreateMonitoredItemsResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, DeleteMonitoredItemsRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, DeleteMonitoredItemsResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<DeleteMonitoredItemsRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<DeleteMonitoredItemsResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, ModifyMonitoredItemsRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, ModifyMonitoredItemsResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<ModifyMonitoredItemsRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<ModifyMonitoredItemsResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, SetMonitoringModeRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, SetMonitoringModeResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<SetMonitoringModeRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<SetMonitoringModeResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, SetTriggeringRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, SetTriggeringResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<SetTriggeringRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<SetTriggeringResponse>& value) = 0;

      virtual void encode(DataBufferPtr& buffer, CallRequest& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void encode(DataBufferPtr& buffer, CallResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CallRequest>& value) = 0;
      virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CallResponse>& value) = 0;

			virtual void decode(DataBufferPtr& buffer, ExpandedNodeId& value) = 0;

			virtual void encodeHeader(DataBufferPtr& buffer, ChannelSecurityToken& token, uint32_t& sequenceHeaderPosition) = 0;

    };
  }
}