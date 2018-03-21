#pragma once
#include <stdint.h>
#include "opcua/Variant.h"

namespace OWA {
	namespace OpcUa {
		struct DataValue {
			DataValue() {};
			DataValue(const DataValue& other);
			~DataValue();
			Variant value;
			StatusCode statusCode;
			DateTime sourceTimestamp;
			uint32_t sourcePicoSeconds;
			DateTime serverTimestamp;
			uint32_t serverPicoSeconds;
		};
	}
}