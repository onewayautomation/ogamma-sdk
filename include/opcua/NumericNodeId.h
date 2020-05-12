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

			AnonymousIdentityToken = 319,
			AnonymousIdentityToken_Encoding_DefaultBinary = 321,
			UserNameIdentityToken = 322,
			UserNameIdentityToken_Encoding_DefaultBinary = 324,
			X509IdentityToken = 325,
			X509IdentityToken_Encoding_DefaultBinary = 327,

			HistoryReadDetails_Encoding_DefaultBinary = 643,
			ReadEventDetails_Encoding_DefaultBinary = 646,
			ReadRawModifiedDetails_Encoding_DefaultBinary = 649,
			ReadProcessedDetails_Encoding_DefaultBinary = 652,
			ReadAtTimeDetails_Encoding_DefaultBinary = 655,
			HistoryData_Encoding_DefaultBinary = 658,
			HistoryEvent_Encoding_DefaultBinary = 661,

			UpdateDataDetails = 680,
			UpdateDataDetails_Encoding_DefaultBinary = 682,
			
			MonitoringFilter = 719,
			MonitoringFilter_Encoding_DefaultBinary = 721,

			DataChangeFilter = 722,
			DataChangeFilter_Encoding_DefaultBinary = 724,

			DataChangeNotification_Encoding_DefaultBinary = 811, 
			NotificationData_Event,
			NotificationData_StatusChange,
			ServerStatusDataType = 864,

			IssuedIdentityToken = 938,
			IssuedIdentityToken_Encoding_DefaultBinary = 940,

			ServerStatusType = 2138,
			ServerStatus = 2256
    };

    const uint8_t BinaryOffset = 2;
  }

}}