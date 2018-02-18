#pragma once
#include <stdint.h>
namespace OWA {
	namespace OpcUa {

		// Configuraable options for requests in corresponding constrructor will be set to some value indicating that it is not set yet.
		// If the caller did not set value, then they will be set in the Connection class to default value for that option.
#define UndefinedInt 0xFFFFFFFF
		inline bool isDefined(uint32_t v) {
			if (v == UndefinedInt) {
				return false;
			}
			return true;
		}
	}
}