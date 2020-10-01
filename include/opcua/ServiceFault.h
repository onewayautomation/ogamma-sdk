#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include "opcua/RequestHeader.h"
#include "opcua/ResponseHeader.h"

namespace OWA {
  namespace OpcUa {
    struct ServiceFaultRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::ServiceFaultRequest;
      }
      RequestHeader header;
    };
    class ServiceFaultResponse {
    public:
      ResponseHeader header;
    };
  }
}