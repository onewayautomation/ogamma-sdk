#pragma once
#include <stdint.h>
#include "opcua/Enums.h"
#include <ctime>
#include <chrono>
#include <string>

namespace OWA {
  namespace OpcUa {
    struct DateTime {
      DateTime():value(0) {};
      DateTime(int64_t v) :value(v) {};
      DateTime(const DateTime& other);
      static DateTime now();
      static std::chrono::nanoseconds nanosecondsFrom1601To1970;
      std::chrono::time_point<std::chrono::system_clock> toChronoTimePoint() const;
      DateTime& operator=(const std::chrono::time_point<std::chrono::system_clock> chronoTimePoint);
      DateTime& operator=(int64_t newValue);
			DateTime& operator=(const DateTime& other);
      bool operator<(const DateTime& other);
      bool operator==(const DateTime& other) const;

      std::string toString(bool localTime = false) const;
			inline bool isSet() { return value > 0; }
      int64_t value;
    };
  }
}