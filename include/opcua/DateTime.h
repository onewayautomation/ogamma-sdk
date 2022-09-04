#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/

#ifndef _WIN32
#ifndef __STDC_LIB_EXT1__
#warning Preprocessor macros  __STDC_LIB_EXT1__ not defined.
#endif
#endif

#ifndef __STDC_WANT_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__ 1
#endif

#include <stdint.h>
#include "opcua/Enums.h"
#include <ctime>
#include <chrono>
#include <string>
#include "boost/date_time/posix_time/posix_time_types.hpp"

namespace OWA {
  namespace OpcUa {
    struct DateTime {
			enum class Precision {
				HundredNanoseconds = 0,
				Microseconds,
				Milliseconds,
				Seconds
			};
      DateTime(void):value(0) {};
      DateTime(int64_t v) :value(v) {};
      DateTime(const DateTime& other);
			
			/* If argument is true, initializes to current time */
			DateTime(bool setToNow);
      
			DateTime& now();
			
			DateTime& fromMillisecondsSinceUnixEpoch(int64_t ms);

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
			bool operator<=(const DateTime& other);
			bool operator>(const DateTime& other);
			bool operator>=(const DateTime& other);
      bool operator==(const DateTime& other) const;

			// Format: YYYY-MM-DDThh:mm:ss.sssZ
			DateTime& parse(const std::string& dateTimeAsString);

      std::string toString(bool localTime = false, bool separateMicroseconds = true, char separator = ' ', char postfix = 0, 
				Precision precision = Precision::HundredNanoseconds) const;
			inline bool isSet() const { return value > 0; }
			
			int64_t toMillisecondsSinceUnixEpoch() const;
			
			static inline std::chrono::milliseconds diffInMilliseconds(const DateTime& startTime, const DateTime& endTime)
			{
				return  (std::chrono::milliseconds) endTime - (std::chrono::milliseconds) startTime;
			}

      static inline uint64_t diffInSeconds(const DateTime& startTime, const DateTime& endTime)
      {
        auto d = ((std::chrono::milliseconds) endTime - (std::chrono::milliseconds) startTime).count();
				d = d / 1000;
				return d;
      }

      static inline uint64_t diffInMinutes(const DateTime& startTime, const DateTime& endTime)
      {
				return diffInSeconds(startTime, endTime) / 60;
      }
      static inline uint64_t diffInHours(const DateTime& startTime, const DateTime& endTime)
      {
        return diffInMinutes(startTime, endTime) / 60;
      }

      static inline uint64_t diffInDays(const DateTime& startTime, const DateTime& endTime)
      {
        return diffInHours(startTime, endTime) / 24; 
      }


      boost::posix_time::ptime toBoostPtime() const;
      OWA::OpcUa::DateTime& fromBoostPtime(const boost::posix_time::ptime& pt);
      
			int64_t value;
    };
  }
}