#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include <stdint.h>
#include "opcua/Variant.h"

namespace OWA {
	namespace OpcUa {
		struct DataValue {
			DataValue();
			DataValue(const DataValue& other);
			DataValue(const Variant& value);
			~DataValue();
			Variant value;
			StatusCode statusCode;
			DateTime sourceTimestamp;
			uint16_t sourcePicoSeconds;
			DateTime serverTimestamp;
			uint16_t serverPicoSeconds;
		};
	}
}