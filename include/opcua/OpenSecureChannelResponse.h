#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include "opcua/ResponseHeader.h"
#include "opcua/ChannelSecurityToken.h"

namespace OWA {
  namespace OpcUa {
    struct OpenSecureChannelResponse {
      OpenSecureChannelResponse() {
        serverProtocolVersion = 0;
      }

      ResponseHeader  header;
      ChannelSecurityToken securityToken;
      ByteString      serverNonce;
      uint32_t        serverProtocolVersion;
			
			typedef std::shared_ptr<OpenSecureChannelResponse> Ptr;
    };
  }
}