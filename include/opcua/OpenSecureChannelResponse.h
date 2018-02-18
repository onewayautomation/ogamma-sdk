#pragma once
#include "opcua/ResponseHeader.h"
#include "opcua/ChannelSecurityToken.h"

namespace OWA {
  namespace OpcUa {
    class OpenSecureChannelResponse {
    public:
      OpenSecureChannelResponse() {
        serverProtocolVersion = 0;
      }

      ResponseHeader  header;
      ChannelSecurityToken securityToken;
      ByteString      serverNonce;
      uint32_t        serverProtocolVersion;
    };
  }
}