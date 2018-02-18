#pragma once
#include "opcua/RequestHeader.h"

namespace OWA {
  namespace OpcUa {
    struct  FindServersRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::FindServersRequest;
      }
      RequestHeader header;
      std::string endpointUrl;
      std::vector<std::string> localeIds;
      std::vector<std::string> serverUris;

			typedef std::shared_ptr<FindServersRequest> Ptr;
    };
  }
}