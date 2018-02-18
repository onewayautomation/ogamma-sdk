#pragma once
#include <string>

namespace OWA {
  class EndpointUrl {
  public:
    EndpointUrl(const std::string& url);
    const std::string& getHostName();
    const std::string& getPortNumber();
  protected:
    std::string hostName;
    std::string portNumber;
  };
}
