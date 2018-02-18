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

namespace OWA {
	namespace OpcUa {
		class Timer;
		namespace Utils{
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
			
			std::vector<ApplicationDescription> selectServers(std::vector<ApplicationDescription>& applications);

			inline std::string statusToString(StatusCode code) {
				return StatusCodeUtil::toString(code);
			}

			void initThreadPool(int numberOfThreads = 1);
			void closeThreadPool();
			std::shared_ptr<ThreadPool> getThreadPool();
			std::shared_ptr<Timer> getTimer();
			std::shared_ptr<boost::asio::io_service> claimIoService();
			void releaseIoService(std::shared_ptr<boost::asio::io_service>& service);
		}
	}
}
