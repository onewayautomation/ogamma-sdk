#pragma once
#include "opcua/ResponseHeader.h"
#include "opcua/ApplicationDescription.h"

namespace OWA {
  namespace OpcUa {
    class FindServersResponse {
    public:
      ResponseHeader header;
      std::vector<ApplicationDescription> servers;
    };
  }
}