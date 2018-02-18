#pragma once
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

      Guid();

      bool operator== (const Guid& r) const;
      bool operator< (const Guid& r) const;
			std::string toString() const;
    };
  }
}
