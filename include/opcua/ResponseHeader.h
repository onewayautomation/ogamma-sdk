#pragma once
#include "opcua/ExtensionObject.h"
#include "opcua/StatusCode.h"
#include "opcua/DiagnosticInfo.h"
#include <vector>

namespace OWA {
  namespace OpcUa {
    struct ResponseHeader
    {
      ResponseHeader();

      DateTime        timestamp;
      uint32_t        requestHandle;
      StatusCode      serviceResult;
      DiagnosticInfo  serviceDiagnostics;
      std::vector<std::string> stringTable;
      ExtensionObject::Ptr additionalHeader;
    };
  }
}