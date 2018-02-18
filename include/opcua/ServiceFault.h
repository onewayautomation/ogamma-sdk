#pragma once
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