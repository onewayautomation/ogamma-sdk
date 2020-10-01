#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include <stdint.h>
#include <string>
namespace OWA {
  namespace OpcUa {
    class Guid {
    public:
      uint32_t Data1;
      uint16_t Data2;
      uint16_t Data3;
      uint8_t  Data4[8];

      // Initialized all members to 0
      Guid();

      // Sets members to newly generated GUID value
      const Guid& Generate();

      bool operator== (const Guid& r) const;
      bool operator!= (const Guid& r) const;
      bool operator< (const Guid& r) const;
			std::string toString() const;
    };
  }
}
