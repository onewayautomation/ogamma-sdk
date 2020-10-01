#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include "opcua/RequestHeader.h"
#include "opcua/ResponseHeader.h"
#include "opcua/ByteString.h"

namespace OWA {
  namespace OpcUa {
    struct CloseSecureChannelRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::CloseSecureChannelRequest;
      }
      RequestHeader header;
      ByteString secureChannelId;

			typedef std::shared_ptr<CloseSecureChannelRequest> Ptr;
    };
    class CloseSecureChannelResponse {
    public:
      ResponseHeader header;
    };
  }
}