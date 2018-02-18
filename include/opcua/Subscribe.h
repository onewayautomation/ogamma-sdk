#pragma once
#include "opcua/RequestHeader.h"
#include "opcua/ResponseHeader.h"
#include "opcua/Enums.h"
#include "opcua/NumericNodeId.h"
#include "opcua/Utils.h"

namespace OWA {
  namespace OpcUa {
		class Connection;

    struct CreateSubscriptionRequest {
			CreateSubscriptionRequest() {
				requestedPublishingInterval = 1000;
				requestedMaxKeepAliveCount = 60;
				
				requestedLifetimeCount = requestedMaxKeepAliveCount * 3;
				maxNotificationsPerPublish = 0;
				publishingEnabled = true;
				priority = 0;

				clientHandle = UndefinedInt;
			}

      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::CreateSubscriptionRequest;
      }
      RequestHeader header;
			Duration requestedPublishingInterval; // 0 or negative means fastest supported interval
			uint32_t requestedLifetimeCount;
			uint32_t requestedMaxKeepAliveCount;
			uint32_t maxNotificationsPerPublish; // 0 - no limit
			bool publishingEnabled;
			uint8_t priority; // 0 - not defined
			
			uint32_t clientHandle;

			typedef std::shared_ptr<CreateSubscriptionRequest> Ptr;
    };

    struct CreateSubscriptionResponse {
			bool isGood() {	return Utils::isGood(*this); }

			CreateSubscriptionResponse() {
				revisedPublishingInterval = 0;
				revisedMaxKeepAliveCount = 0;
				revisedLifetimeCount = 0;
				subscriptionId = 0;
			}

      ResponseHeader header;
			uint32_t subscriptionId;
			Duration revisedPublishingInterval;
			uint32_t revisedLifetimeCount;
			uint32_t revisedMaxKeepAliveCount;
    };

    struct ModifySubscriptionRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::ModifySubscriptionRequest;
      }
      RequestHeader header;

			typedef std::shared_ptr<ModifySubscriptionRequest> Ptr;
    };

    struct ModifySubscriptionResponse {
      ResponseHeader header;
    };

    struct DeleteSubscriptionsRequest {
			friend Connection;

      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::DeleteSubscriptionsRequest;
      }
      RequestHeader header;
			std::vector<uint32_t> subscriptionIds;

			typedef std::shared_ptr<DeleteSubscriptionsRequest> Ptr;

			std::vector<uint32_t> internalds;
    };

    struct DeleteSubscriptionsResponse {
			inline bool isGood() {
				return Utils::isGood(*this);
			}
      ResponseHeader header;
			std::vector<StatusCode> results;
			std::vector<DiagnosticInfo> diagnosticInfos;
    };

    struct TransferSubscriptionsRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::TransferSubscriptionsRequest;
      }
      RequestHeader header;
			typedef std::shared_ptr<TransferSubscriptionsRequest> Ptr;
    };

    struct TransferSubscriptionsResponse {
      ResponseHeader header;
    };

    struct SetPublishingModeRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::SetPublishingModeRequest;
      }
      RequestHeader header;
			typedef std::shared_ptr<SetPublishingModeRequest> Ptr;
    };

    struct SetPublishingModeResponse {
      ResponseHeader header;
    };

		struct SubscriptionAcknowledgement {
			SubscriptionAcknowledgement() {
				subscriptionId = 0;
				sequenceNumber = 0;
			}
			SubscriptionAcknowledgement(uint32_t subId, uint32_t seqNum) {
				subscriptionId = subId;
				sequenceNumber = seqNum;
			}

			uint32_t subscriptionId;
			uint32_t sequenceNumber;
		};


    struct PublishRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::PublishRequest;
      }
      RequestHeader header;
			std::vector<SubscriptionAcknowledgement> subscriptionAcknowledgements;
			typedef std::shared_ptr<PublishRequest> Ptr;
			~PublishRequest() {
				int i = 0;
			}
    };

		struct NotificationMessage {
			uint32_t sequenceNumber;
			DateTime publishTime;
			std::vector<ExtensionObject::Ptr> notificationData;
		};

    struct PublishResponse {
      ResponseHeader header;
			uint32_t subscriptionId;
			std::vector<uint32_t> availableSequenceNumbers;
			bool moreNotifications;
			NotificationMessage notificationMessage;
			std::vector<StatusCode> results;
			std::vector<DiagnosticInfo> diagnosticInfos;

			inline bool isGood() { return Utils::isGood(*this); } 
    };

    struct RepublishRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::RepublishRequest;
      }
      RequestHeader header;
			typedef std::shared_ptr<RepublishRequest> Ptr;
    };
    struct RepublishResponse {
      ResponseHeader header;
    };
  }
}