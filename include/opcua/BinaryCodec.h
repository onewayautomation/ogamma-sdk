#pragma once
#include "opcua/Cryptor.h"
#include "opcua/Codec.h"
#include "opcua/MessageHeader.h"
#include "opcua/Variant.h"
#include "opcua/DataChangeNotification.h"

namespace OWA {
  namespace OpcUa {
      class BinaryCodec : public Codec {
      public:
        BinaryCodec(std::shared_ptr<Cryptor>& cryptor);
        void encode(DataBufferPtr& buffer, HelloMessage& body);

        void encode(DataBufferPtr& buffer, OpenSecureChannelRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        void encode(DataBufferPtr& buffer, OpenSecureChannelResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);

        virtual void decode(DataBufferPtr&  buffer, std::shared_ptr<MessageHeader>& result);
        virtual void decode(DataBufferPtr&  buffer, std::shared_ptr<HelloMessage>& result);
        virtual void decode(DataBufferPtr&  buffer, std::shared_ptr<AchnowledgeMessage>& result);

        virtual void decode(DataBufferPtr&  buffer, std::shared_ptr<ErrorMessage>& result);

        void decode(DataBufferPtr& buffer, OpenSecureChannelRequest& value);
        void decode(DataBufferPtr& buffer, std::shared_ptr<OpenSecureChannelResponse>& value);

        void decode(TcpBinaryMessage&, std::shared_ptr<AssymmetricAlgortrithmSecurityHeader>& value);
        void decode(TcpBinaryMessage&, std::shared_ptr<SymmetricAlgorithmSecurityHeader>& value);
        void decode(TcpBinaryMessage&, std::shared_ptr<SymmetricCryptoContext>& context, std::shared_ptr<SymmetricAlgorithmSecurityHeader>& value);

        void encode(DataBufferPtr& buffer, FindServersRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        void encode(DataBufferPtr& buffer, FindServersResponse& value, std::shared_ptr<SymmetricCryptoContext>& context) {};
        void decode(DataBufferPtr& buffer, std::shared_ptr<FindServersRequest>& value) {};
        void decode(DataBufferPtr& buffer, std::shared_ptr<FindServersResponse>& value);

        void encode(DataBufferPtr& buffer, GetEndpointsRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        void encode(DataBufferPtr& buffer, GetEndpointsResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        void decode(DataBufferPtr& buffer, std::shared_ptr<GetEndpointsRequest>& value);
        void decode(DataBufferPtr& buffer, std::shared_ptr<GetEndpointsResponse>& value);

        virtual void encode(DataBufferPtr& buffer, CloseSecureChannelRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, CloseSecureChannelResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CloseSecureChannelRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CloseSecureChannelResponse>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<ServiceFaultResponse>& value);

        virtual void encode(DataBufferPtr& buffer, CreateSessionRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, CreateSessionResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CreateSessionRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CreateSessionResponse>& value);

        virtual void encode(DataBufferPtr& buffer, ActivateSessionRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, ActivateSessionResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<ActivateSessionRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<ActivateSessionResponse>& value);

        virtual void encode(DataBufferPtr& buffer, BrowseRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, BrowseResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<BrowseRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<BrowseResponse>& value);

        virtual void encode(DataBufferPtr& buffer, BrowseNextRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, BrowseNextResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<BrowseNextRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<BrowseNextResponse>& value);

        virtual void encode(DataBufferPtr& buffer, TranslateBrowsePathsToNodeIdsRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, TranslateBrowsePathsToNodeIdsResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<TranslateBrowsePathsToNodeIdsRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<TranslateBrowsePathsToNodeIdsResponse>& value);

        virtual void encode(DataBufferPtr& buffer, ReadRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, ReadResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<ReadRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<ReadResponse>& value);

        virtual void encode(DataBufferPtr& buffer, WriteRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, WriteResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<WriteRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<WriteResponse>& value);

        virtual void encode(DataBufferPtr& buffer, HistoryReadRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, HistoryReadResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<HistoryReadRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<HistoryReadResponse>& value);

        virtual void encode(DataBufferPtr& buffer, HistoryUpdateRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, HistoryUpdateResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<HistoryUpdateRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<HistoryUpdateResponse>& value);

        virtual void encode(DataBufferPtr& buffer, CreateSubscriptionRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, CreateSubscriptionResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CreateSubscriptionRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CreateSubscriptionResponse>& value);

        virtual void encode(DataBufferPtr& buffer, ModifySubscriptionRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, ModifySubscriptionResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<ModifySubscriptionRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<ModifySubscriptionResponse>& value);

        virtual void encode(DataBufferPtr& buffer, DeleteSubscriptionsRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, DeleteSubscriptionsResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<DeleteSubscriptionsRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<DeleteSubscriptionsResponse>& value);

        virtual void encode(DataBufferPtr& buffer, TransferSubscriptionsRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, TransferSubscriptionsResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<TransferSubscriptionsRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<TransferSubscriptionsResponse>& value);

        virtual void encode(DataBufferPtr& buffer, SetPublishingModeRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, SetPublishingModeResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<SetPublishingModeRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<SetPublishingModeResponse>& value);

        virtual void encode(DataBufferPtr& buffer, PublishRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, PublishResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<PublishRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<PublishResponse>& value);

        virtual void encode(DataBufferPtr& buffer, RepublishRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, RepublishResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<RepublishRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<RepublishResponse>& value);

        virtual void encode(DataBufferPtr& buffer, CreateMonitoredItemsRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, CreateMonitoredItemsResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CreateMonitoredItemsRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CreateMonitoredItemsResponse>& value);

        virtual void encode(DataBufferPtr& buffer, DeleteMonitoredItemsRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, DeleteMonitoredItemsResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<DeleteMonitoredItemsRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<DeleteMonitoredItemsResponse>& value);

        virtual void encode(DataBufferPtr& buffer, ModifyMonitoredItemsRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, ModifyMonitoredItemsResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<ModifyMonitoredItemsRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<ModifyMonitoredItemsResponse>& value);

        virtual void encode(DataBufferPtr& buffer, SetMonitoringModeRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, SetMonitoringModeResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<SetMonitoringModeRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<SetMonitoringModeResponse>& value);

        virtual void encode(DataBufferPtr& buffer, SetTriggeringRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, SetTriggeringResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<SetTriggeringRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<SetTriggeringResponse>& value);

        virtual void encode(DataBufferPtr& buffer, CallRequest& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void encode(DataBufferPtr& buffer, CallResponse& value, std::shared_ptr<SymmetricCryptoContext>& context);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CallRequest>& value);
        virtual void decode(DataBufferPtr& buffer, std::shared_ptr<CallResponse>& value);

      protected:
        void encode(DataBufferPtr& buffer, const uint8_t& value);
        void encode(DataBufferPtr& buffer, const int8_t& value);

        void encode(DataBufferPtr& buffer, const uint16_t& value);
        void encode(DataBufferPtr& buffer, const int16_t& value);

        void encode(DataBufferPtr& buffer, const uint32_t& value);
        void encode(DataBufferPtr& buffer, const int32_t& value);
        void encode(DataBufferPtr& buffer, const std::string& value);
        void encode(DataBufferPtr& buffer, const ByteString& value);
        void encode(DataBufferPtr& buffer, const RequestHeader& value);
        void encode(DataBufferPtr& buffer, const ExpandedNodeId& value);
        void encode(DataBufferPtr& buffer, const NodeId& value);
        void encode(DataBufferPtr& buffer, const NodeId& value, uint8_t encodingByte);
        void encode(DataBufferPtr& buffer, const Guid& value);
        void encode(DataBufferPtr& buffer, const DateTime& value);
				void encode(DataBufferPtr& buffer, const ExtensionObject::Ptr& value);
        void encode(DataBufferPtr& buffer, const int64_t& value);
        void encode(DataBufferPtr& buffer, const uint64_t& value);
        void encode(DataBufferPtr& buffer, const LocalizedText& value);
        template<typename Type>
        void encode(DataBufferPtr& buffer, const std::vector<Type>& value) {
          if (value.empty()) {
            encode(buffer, (int32_t)-1);
          }
          else {
            encode(buffer, (int32_t)value.size());
            for (size_t index = 0; index < value.size(); index++) {
              encode(buffer, value[index]);
            }
          }
        }

        void encode(DataBufferPtr&buffer, const ApplicationDescription& value);
        void encode(DataBufferPtr&buffer, const Duration& value);
        void encode(DataBufferPtr& buffer, const SignatureData& value);
        void encode(DataBufferPtr& buffer, const AnonymousIdentityToken& value);
				void encode(DataBufferPtr& buffer, TimestampsToReturn value);
				void encode(DataBufferPtr& buffer, const ReadValueId& value);
				void encode(DataBufferPtr& buffer, const QualifiedName& value);
				void encode(DataBufferPtr& buffer, const ViewDescription& value);
				void encode(DataBufferPtr& buffer, const BrowseDescription& value);
				void encode(DataBufferPtr& buffer, bool value);
				void encode(DataBufferPtr& buffer, const MonitoredItemCreateRequest& value);
				void encode(DataBufferPtr& buffer, const MonitoringParameters& value);
				void encode(DataBufferPtr& buffer, const SubscriptionAcknowledgement& value);

        void decode(DataBufferPtr& buffer, uint8_t& value);
        void decode(DataBufferPtr& buffer, int8_t& value);
        void decode(DataBufferPtr& buffer, uint16_t& value);
        void decode(DataBufferPtr& buffer, int16_t& value);
        void decode(DataBufferPtr& buffer, uint32_t& value);
        void decode(DataBufferPtr& buffer, int32_t& value);
        void decode(DataBufferPtr& buffer, uint64_t& value);
        void decode(DataBufferPtr& buffer, int64_t& value);
        void decode(DataBufferPtr& buffer, std::string& value);
        void decode(DataBufferPtr& buffer, Duration& value);
        void decode(DataBufferPtr& buffer, SignatureData& value);

        void decode(DataBufferPtr& buffer, ByteString& value);
        void decode(DataBufferPtr& buffer, ResponseHeader& value);
        void decode(DataBufferPtr& buffer, SequenceHeader& value);
        void decode(DataBufferPtr& buffer, ExpandedNodeId& value);
        uint8_t decode(DataBufferPtr& buffer, NodeId& value);
        void decode(DataBufferPtr& buffer, Guid& value);
        void decode(DataBufferPtr& buffer, DateTime& value);
        void decode(DataBufferPtr& buffer, DiagnosticInfo& value);
        void decode(DataBufferPtr& buffer, LocalizedText& value);
        void decode(DataBufferPtr& buffer, ApplicationDescription& value);
        void decode(DataBufferPtr& buffer, EndpointDescription& value);
        void decode(DataBufferPtr& buffer, UserTokenPolicy& value);
        void decode(DataBufferPtr& buffer, StatusCode& value);
				void decode(DataBufferPtr& buffer, DataValue& value);
				void decode(DataBufferPtr& buffer, Variant& value);
				void decode(DataBufferPtr& buffer, float& value);
				void decode(DataBufferPtr& buffer, QualifiedName& value);
				void decode(DataBufferPtr& buffer, bool& value);
				void decode(DataBufferPtr& buffer, BrowseResult& value);
				void decode(DataBufferPtr& buffer, ReferenceDesciption& value);
				void decode(DataBufferPtr& buffer, MonitoredItemCreateResult& value);
				void decode(DataBufferPtr& buffer, NotificationMessage& value);
				void decode(DataBufferPtr& buffer, ExtensionObject::Ptr& value);
				void decode(DataBufferPtr& buffer, MonitoredItemNotification& value);

        template<typename RequestType>
        void encodeHeader(DataBufferPtr& buffer, ChannelSecurityToken& token, RequestType& value, uint32_t& sequenceHeaderPosition) {
          // 1. Initialize buffer:
          if (buffer.get() == 0) {
            buffer.reset(new DataBuffer);
          }

          // 2. Serialize message header:
          MessageHeader header(TcpMessageType::MSG);
          buffer->sputn((const char*)&header, sizeof(MessageHeader));

          encode(buffer, token.SecureChannelId);

          // 3. Serialize Symmetric Algorithm Security Header (SASH)
          encode(buffer, token.TokenId);
          
          sequenceHeaderPosition = buffer->size();

          // 4. Serialize SequenceHeader:
          SequenceHeader sequenceHeader(value.header.requestHandle, sequenceNumber);
          sequenceNumber++;
          encode(buffer, sequenceHeader.RequestId);
          encode(buffer, sequenceHeader.SequenceNumber);

          // 5. Serialize body with request:
          // First, serialize type id for this request:
          encode(buffer, ExpandedNodeId::getId(RequestType::getTypeId()));
          // And then actual body:
          encode(buffer, value.header);
        }

        template<typename ArrayElementType>
        void decode(DataBufferPtr& buffer, std::vector<ArrayElementType>& value) {
          value.clear();
          int32_t length;
          decode(buffer, length);
          if (length > 0) {
            value.reserve(length);
          }
          for (int index = 0; index < length; index++) {
            ArrayElementType entry;
            decode(buffer, entry);
            value.push_back(entry);
          }
        }

        std::shared_ptr<Cryptor> cryptor;
        uint32_t sequenceNumber;
      };
    }
  }
