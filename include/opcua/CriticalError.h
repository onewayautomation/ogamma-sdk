#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include <stdexcept>
namespace OWA {
  namespace OpcUa {
    class CriticalErrorException : public std::runtime_error
    {
    public:
      explicit CriticalErrorException(const std::string& what_arg);
      explicit CriticalErrorException(const char* what_arg);
      virtual ~CriticalErrorException();
    };
  }
}