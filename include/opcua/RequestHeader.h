#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include "opcua/opcua.h"
#include "opcua/DateTime.h"
#include "opcua/ExtensionObject.h"
#include "opcua/StatusCode.h"

namespace OWA {
  namespace OpcUa {
    struct RequestHeader
    {
			friend class Connection;
			friend class TcpTransport;
      RequestHeader();
			uint32_t getRequestId();

      NodeId          sessionAuthenticationToken;
      DateTime        utcTime;
      uint32_t        requestHandle;
      uint32_t        returnDiagnostics;
      std::string     auditEntryId;
      uint32_t        timeoutHint; // in milliseconds
      ExtensionObject::Ptr additionalHeader;
		private:
			uint32_t requestId; // Assigned by SDK and used by SDK. Used to map received responses to sent requests. 
    public:
      // Used internally by he SDK.
      StatusCode		  sendResult;
      uint32_t sequenceNumber; // Helper field, equal to the message sequence header. Used foir debugging / diagnostics only.
    };
  }
}
