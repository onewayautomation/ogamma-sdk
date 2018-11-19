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
      static std::chrono::microseconds microSecondsFrom1601To1970;
      std::chrono::time_point<std::chrono::system_clock> toChronoTimePoint() const;
      DateTime& operator=(const std::chrono::time_point<std::chrono::system_clock> chronoTimePoint);
			
			DateTime& operator+=(const std::chrono::microseconds& diff);
			DateTime& operator+=(const std::chrono::milliseconds& diff);
			DateTime& operator+=(const std::chrono::seconds& diff);
			DateTime operator+(const std::chrono::microseconds& diff);
			DateTime operator+(const std::chrono::milliseconds& diff);
			DateTime operator+(const std::chrono::seconds& diff);
			DateTime operator-(const std::chrono::microseconds& diff);
			DateTime operator-(const std::chrono::milliseconds& diff);
			DateTime operator-(const std::chrono::seconds& diff);

			DateTime& operator=(int64_t newValue);
			DateTime& operator=(const DateTime& other);

			operator std::chrono::microseconds() const;
			operator std::chrono::milliseconds() const;
			operator std::chrono::seconds() const;

			// DateTime operator-(const DateTime& other) const;

      bool operator<(const DateTime& other);
      bool operator==(const DateTime& other) const;

			bool parse(const std::string& dateTimeAsString);

      std::string toString(bool localTime = false) const;
			inline bool isSet() { return value > 0; }
      int64_t value;
    };
  }
}