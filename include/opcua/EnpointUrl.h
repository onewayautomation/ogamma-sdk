#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include <string>

namespace OWA {
  class EndpointUrl {
  public:
    EndpointUrl(const std::string& url);
    const std::string& getHostName();
		void setHostName(const std::string& newHostName);
    const std::string& getPortNumber();
		std::string toString();
  protected:
    std::string hostName;
    std::string portNumber;
  };
}
