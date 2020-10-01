#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include "opcua/RequestHeader.h"
#include "opcua/ResponseHeader.h"
#include "opcua/Enums.h"
#include "opcua/NumericNodeId.h"
#include "opcua/Attribute.h"

namespace OWA {
  namespace OpcUa {
		class MonitoringFilter : public ExtensionObject
		{
			public:
				MonitoringFilter() {};
		};

		enum struct DataChangeTrigger: uint8_t 
		{
			STATUS_0 = 0, // Report a notification ONLY if the StatusCode associated with the value changes.See Table 178 for StatusCodes defined in this standard.OPC 10000 - 8 specifies additional StatusCodes that are valid in particular for device data.
			STATUS_VALUE_1 = 1, /* Report a notification if either the StatusCode or the value change.The Deadband filter can be used in addition for filtering value changes.
			For floating point values a Server shall check for NaN and only report a single notification with NaN when the value enters the NaN state.
			This is the default setting if no filter is set. */
			STATUS_VALUE_TIMESTAMP_2 = 2 
		};

		enum struct DeadbandType:uint8_t {
			DadbandType_None_0, // No Dead-band calculation should be applied.
			DadbandType_Absolute_1, // AbsoluteDeadband
			DadbandType_Percent_2
		};

		class DataChangeFilter : public MonitoringFilter
		{
		public:
			DataChangeFilter();
			DataChangeTrigger trigger;
			DeadbandType deadbandType;
			double deadbandValue;
		};

		struct MonitoringParameters 
    {
			MonitoringParameters(Duration samplingInterval = -1, uint32_t queueSize = 2);
			uint32_t clientHandle;
			Duration samplingInterval; // 0 - fastest; -1  - the same as Publishing interval
			ExtensionObject::Ptr filter;
			uint32_t queueSize;
			bool discurdOldest;
		};

		struct MonitoredItemCreateRequest {
			MonitoredItemCreateRequest();
			MonitoredItemCreateRequest(const NodeId& nodeId, Duration samplingInterval = -1, uint32_t queueSize = 2);
			ReadValueId itemToMonitor;
			MonitoringMode monitoringMode;
			MonitoringParameters monitoringParameters;
		};

		struct MonitoredItemCreateResult {
			MonitoredItemCreateResult() :statusCode(StatusCode::BadInternalError),monitoredItemId(0), revisedSamplingInterval(0),revisedQueueSize(0) {};
			StatusCode statusCode;
			uint32_t monitoredItemId;
			Duration revisedSamplingInterval;
			uint32_t revisedQueueSize;
			ExtensionObject::Ptr filterResult;
		};

    struct CreateMonitoredItemsRequest {
			static RequestResponseTypeId getTypeId();

			CreateMonitoredItemsRequest();
      RequestHeader header;
			uint32_t subscriptionId;
			uint32_t actualServerSideSubscriptionId;
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
			static RequestResponseTypeId getTypeId();
      RequestHeader header;
			boost::any		context;
			typedef std::shared_ptr<ModifyMonitoredItemsRequest> Ptr;
    };
    struct ModifyMonitoredItemsResponse {
      ResponseHeader header;

			typedef std::shared_ptr<ModifyMonitoredItemsResponse> Ptr;
    };

    struct DeleteMonitoredItemsRequest {
			static RequestResponseTypeId getTypeId();
			DeleteMonitoredItemsRequest();
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
			static RequestResponseTypeId getTypeId();

      RequestHeader header;
			boost::any		context;
			typedef std::shared_ptr<SetTriggeringRequest> Ptr;
    };
    struct SetTriggeringResponse {
      ResponseHeader header;
			typedef std::shared_ptr<SetTriggeringResponse> Ptr;
    };

    struct SetMonitoringModeRequest {
			static RequestResponseTypeId getTypeId();

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