#pragma once
#include "opcua/ExtensionObject.h"
#include "opcua/StatusCode.h"
#include "opcua/DiagnosticInfo.h"
#include <vector>

namespace OWA {
  namespace OpcUa {
		class Transport;
    struct ResponseHeader
    {
			friend class Connection;
			friend class TcpTransport;

      ResponseHeader();

      DateTime        timestamp;
      uint32_t        requestHandle; // User assigned handle.
      StatusCode      serviceResult;
      DiagnosticInfo  serviceDiagnostics;
      std::vector<std::string> stringTable;
      ExtensionObject::Ptr additionalHeader;
		private:
			uint32_t				requestId; // SDK assigned id, Actually it is part of sequence header, but in order to provide access to it from Connection class, placed here.
		};
  }
}