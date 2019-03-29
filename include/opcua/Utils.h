#pragma once
#include "opcua/StatusCode.h"
#include "opcua/FindServersResponse.h"
#include "opcua/GetEndpoints.h"
#include "opcua/OpenSecureChannelResponse.h"
#include "opcua/Session.h"
#include "opcua/ApplicationDescription.h"
#include "opcua/ThreadPool.h"
#include "opcua/Timer.h"
#include "opcua/ioService.h"
#include "opcua/Enums.h"
#include <sstream>
#include <chrono>
#include <boost/date_time/posix_time/ptime.hpp>

namespace OWA {
	namespace OpcUa {
		class Timer;
		struct EndpointDescription;

		namespace Utils
		{
			inline bool isGood(StatusCode code) {
				if ((((uint32_t)code) & 0x80000000) == 0) {
					return true;
				}
				return false;
			}

			template<typename ResponseType>
			bool isGood(const ResponseType& response) {
				return isGood(response.header.serviceResult);
			}

			template<typename ResponseType>
			bool isGood(const std::shared_ptr<ResponseType>& response) {
				return isGood(*response);
			}			
			
			inline bool isGood(uint32_t code)
			{
				return isGood((StatusCode)code);
			}

			std::vector<ApplicationDescription> selectServers(std::vector<ApplicationDescription>& applications);

			inline std::string toString(StatusCode code) {
				return StatusCodeUtil::toString(code);
			}

			inline std::string statusToString(StatusCode code) {
				return StatusCodeUtil::toString(code);
			}

			void initThreadPool(int numberOfThreads = 1);
			void closeThreadPool();
			// Returns time given ms from now.
			std::chrono::time_point<std::chrono::steady_clock> getTimeNowPlusMilliSeconds(uint32_t ms);
			std::shared_ptr<ThreadPool> getThreadPool();
			
			void initCallbackThreadPool(int numberOfThreads = 1);
			std::shared_ptr<ThreadPool> getCallbackThreadPool();
			void closeCallbackThreadPool();

			std::shared_ptr<Timer> getTimer();
			std::shared_ptr<boost::asio::io_service> claimIoService();
			void releaseIoService(std::shared_ptr<boost::asio::io_service>& service);

			void closeSdk();

			std::string toString(OpcUa::SecurityMode mode, const std::string& locale = "en");
			std::string toString(OpcUa::MessageSecurityMode mode, const std::string& locale = "en");
			std::string toString(const OpcUa::EndpointDescription& ed);
			std::string toString(const OpcUa::UserIdentityTokenType uitt);
			std::string toString(const OpcUa::ConnectionState state, const std::string& locale = "en");
			std::string toString(const OpcUa::ByteString& value);
			
			std::string currentTimestamp();
			std::string toLower(const std::string& str);
			bool startsWith(const std::string& str, const std::string& subs);


			boost::posix_time::ptime convert(const std::chrono::system_clock::time_point& from);

			std::chrono::system_clock::time_point convert(const boost::posix_time::ptime& from);

		}
	}
}