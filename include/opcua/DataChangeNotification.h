#pragma once
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
