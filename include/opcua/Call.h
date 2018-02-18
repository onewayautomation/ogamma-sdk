#pragma once
#include "opcua/RequestHeader.h"
#include "opcua/ResponseHeader.h"
#include "opcua/Enums.h"
#include "opcua/NumericNodeId.h"

namespace OWA {
  namespace OpcUa {
    struct CallRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::CallRequest;
      }
      RequestHeader header;
			typedef std::shared_ptr<CallRequest> Ptr;
    };
    struct CallResponse {
      ResponseHeader header;

			std::shared_ptr<CallResponse> Ptr;
    };
  }
}