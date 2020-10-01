#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include "opcua/RequestHeader.h"
#include <boost/any.hpp>

namespace OWA {
  namespace OpcUa {
    struct  FindServersRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::FindServersRequest;
      }
      RequestHeader header;
			boost::any		context;
      std::string endpointUrl;
      std::vector<std::string> localeIds;
      std::vector<std::string> serverUris;

			typedef std::shared_ptr<FindServersRequest> Ptr;
    };
  }
}