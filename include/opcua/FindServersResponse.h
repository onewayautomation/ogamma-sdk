#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include "opcua/ResponseHeader.h"
#include "opcua/ApplicationDescription.h"

namespace OWA {
  namespace OpcUa {
    class FindServersResponse {
    public:
      ResponseHeader header;
      std::vector<ApplicationDescription> servers;

      typedef std::shared_ptr<FindServersResponse> Ptr;
    };
  }
}