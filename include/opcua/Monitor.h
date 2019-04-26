#pragma once
#include "opcua/RequestHeader.h"
#include "opcua/ResponseHeader.h"
#include "opcua/Enums.h"
#include "opcua/NumericNodeId.h"

namespace OWA {
  namespace OpcUa {
		struct MonitoringParameters 
    {
      MonitoringParameters(Duration samplingInterval = -1, uint32_t queueSize = 2) 
      {
        clientHandle = UndefinedInt;
        this->samplingInterval = samplingInterval;
        this->queueSize = queueSize;
        discurdOldest = true;
      }
			uint32_t clientHandle;
			Duration samplingInterval; // 0 - fastest; -1  - the same as Publishing interval
			ExtensionObject::Ptr filter;
			uint32_t queueSize;
			bool discurdOldest;
		};

		struct MonitoredItemCreateRequest {
			MonitoredItemCreateRequest() {
				monitoringMode = MonitoringMode::reporting;
			}
			MonitoredItemCreateRequest(const NodeId& nodeId, Duration samplingInterval = -1, uint32_t queueSize = 2): 
        itemToMonitor(nodeId), monitoringParameters(samplingInterval, queueSize)
      {
				monitoringMode = MonitoringMode::reporting;
			}
			ReadValueId itemToMonitor;
			MonitoringMode monitoringMode;
			MonitoringParameters monitoringParameters;
		};

		struct MonitoredItemCreateResult {
			StatusCode statusCode;
			uint32_t monitoredItemId;
			Duration revisedSamplingInterval;
			uint32_t revisedQueueSize;
			ExtensionObject::Ptr filterResult;
		};

    struct CreateMonitoredItemsRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::CreateMonitoredItemsRequest;
      }
			CreateMonitoredItemsRequest() {
				subscriptionId = 0;
				timestamsToReturn = TimestampsToReturn::Both;
				internalId = UndefinedInt;
			}
      RequestHeader header;
			uint32_t subscriptionId;
			uint32_t internalId;
			TimestampsToReturn timestamsToReturn;
			std::vector<MonitoredItemCreateRequest> itemsToCreate;
			boost::any		context;
			typedef std::shared_ptr<CreateMonitoredItemsRequest> Ptr;
    };
    struct CreateMonitoredItemsResponse {
      ResponseHeader header;
			std::vector<MonitoredItemCreateResult> results;
			std::vector<DiagnosticInfo> diagnosticInfos;
			inline bool isGood() { return Utils::isGood(*this); }

			typedef std::shared_ptr<CreateMonitoredItemsResponse> Ptr;
    };

    struct ModifyMonitoredItemsRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::ModifyMonitoredItemsRequest;
      }
      RequestHeader header;
			boost::any		context;
			typedef std::shared_ptr<ModifyMonitoredItemsRequest> Ptr;
    };
    struct ModifyMonitoredItemsResponse {
      ResponseHeader header;

			typedef std::shared_ptr<ModifyMonitoredItemsResponse> Ptr;
    };

    struct DeleteMonitoredItemsRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::DeleteMonitoredItemsRequest;
      }
			DeleteMonitoredItemsRequest() {
				subscriptionId = 0;
				internalData1 = UndefinedInt;
			}
      RequestHeader header;
			uint32_t subscriptionId;
			std::vector<uint32_t> monitoredItemIds;

			uint32_t internalData1;
			std::vector<uint32_t> internalData2;
			boost::any		context;

			typedef std::shared_ptr<DeleteMonitoredItemsRequest> Ptr;
    };

    struct DeleteMonitoredItemsResponse {
      ResponseHeader header;
			std::vector<StatusCode> results;
			std::vector<DiagnosticInfo> diagnosticInfos;
			inline bool isGood() { return Utils::isGood(*this); }

			typedef std::shared_ptr<DeleteMonitoredItemsResponse> Ptr;
    };

    struct SetTriggeringRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::SetTriggeringRequest;
      }
      RequestHeader header;
			boost::any		context;
			typedef std::shared_ptr<SetTriggeringRequest> Ptr;
    };
    struct SetTriggeringResponse {
      ResponseHeader header;
			typedef std::shared_ptr<SetTriggeringResponse> Ptr;
    };

    struct SetMonitoringModeRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::SetMonitoringModeRequest;
      }
      RequestHeader header;
			boost::any		context;
			typedef std::shared_ptr<SetMonitoringModeRequest> Ptr;
    };
    struct SetMonitoringModeResponse {
      ResponseHeader header;
			typedef std::shared_ptr<SetMonitoringModeResponse> Ptr;
    };
  }
}