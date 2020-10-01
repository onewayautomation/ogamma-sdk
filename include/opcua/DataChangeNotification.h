#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include "opcua/ExtensionObject.h"
#include "opcua/DataValue.h"

namespace OWA {
	namespace OpcUa {
		struct MonitoredItemNotification {
			MonitoredItemNotification() :clientHandle(0) {};
			uint32_t clientHandle;
			DataValue dataValue;
		};
		struct DataChangeNotification : public ExtensionObject {
		public:
			DataChangeNotification();
			std::vector<MonitoredItemNotification> monitoredItems;
			std::vector<DiagnosticInfo> diagnosticInfos;
		};
	}
}
