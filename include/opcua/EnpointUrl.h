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
    EndpointUrl();
    EndpointUrl(const std::string& url);
    const std::string& getHostName();
		void setHostName(const std::string& newHostName);
    
    void setPortNumber(const std::string& p);
    const std::string& getPortNumber();
    uint32_t getNumericPortNumber();

    void setPath(const std::string& p);
    std::string getPath() const;
		std::string toString();

    bool hasTrailingSlash;

    bool empty();

  protected:
    std::string hostName;
    std::string portNumber;
    std::string path;
  };
}
