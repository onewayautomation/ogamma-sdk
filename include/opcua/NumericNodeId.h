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
			
			HistoryReadDetails_Encoding_DefaultBinary = 643,
			ReadEventDetails_Encoding_DefaultBinary = 646,
			ReadRawModifiedDetails_Encoding_DefaultBinary = 649,
			ReadProcessedDetails_Encoding_DefaultBinary = 652,
			ReadAtTimeDetails_Encoding_DefaultBinary = 655,
			HistoryData_Encoding_DefaultBinary = 658,
			HistoryEvent_Encoding_DefaultBinary = 661,

			DataChangeNotification_Encoding_DefaultBinary = 811, 
			NotificationData_Event,
			NotificationData_StatusChange,
			ServerStatusDataType = 864,
			ServerStatusType = 2138,
			ServerStatus = 2256
    };

    const uint8_t BinaryOffset = 2;
  }

}}