#pragma once
#include "opcua/opcua.h"
#include "opcua/DateTime.h"
#include "opcua/ExtensionObject.h"


namespace OWA {
  namespace OpcUa {
    struct RequestHeader
    {
      RequestHeader();

      NodeId          sessionAuthenticationToken;
      DateTime        utcTime;
      uint32_t        requestHandle;
      uint32_t        returnDiagnostics;
      std::string     auditEntryId;
      uint32_t        timeoutHint; // in milliseconds
      ExtensionObject::Ptr additionalHeader;
    };
  }
}
