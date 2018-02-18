#pragma once
#include <stdint.h>

namespace OWA { namespace OpcUa {
  namespace Ids {

		enum NumericNodeId {
			Empty = 0,
			ByteString = 15,
			XmlElement = 16,
			HierarchicalReferences = 33,
			RootFolder = 84,
			AnonymousIdentityToken = 321,
			UserNameIdentityToken_Encoding_DefaultBinary = 324,
			DataChangeNotification_Encoding_DefaultBinary = 811, 
			NotificationData_Event,
			NotificationData_StatusChange
    };

    const uint8_t BinaryOffset = 2;
  }

}}