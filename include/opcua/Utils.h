#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
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

#include <spdlog/spdlog.h>

// In Windows, error code 995 means it was canceled (not actual timeout happened).
// In Linux Ubuntu 18.04 error code in case of cancellation os 125.
#ifdef WIN32
#define CANCEL_CODE 995
#else
#define CANCEL_CODE 125
#endif

namespace OWA {
	namespace OpcUa {
		class Timer;
		struct EndpointDescription;
		class Connection;
		class BinaryCodec;

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

			inline std::string toString(StatusCode code, bool includeCode = false) {
				return StatusCodeUtil::toString(code, includeCode);
			}

			inline std::string statusToString(StatusCode code, bool includeCode = false) {
				return StatusCodeUtil::toString(code, includeCode);
			}

      void initSdk(
				const std::string& logFileName = "./data/logs/OpcUaSdk-client-Log.txt", 
				uint16_t level = 0,
        int numberOfIoThreads = 1, 
				int numberOfCallbackThreads = 2,
				int numberOfTimerThreads = 2,
				size_t maxFileSize = 8*1024*1024, 
				size_t maxFileNumber = 10);

      void initSdk(
				int numberOfIoThreads, 
				int numberOfCallbackThreads, 
				int numberOfTimerThreads,
				const std::string& logFileName, 
				uint16_t level, 	
				size_t maxFileSize, 
				size_t maxFileNumber);

			// Returns time given ms from now.
			std::chrono::time_point<std::chrono::steady_clock> getTimeNowPlusMilliSeconds(uint32_t ms);
			
			std::shared_ptr<Timer> getTimer();

			void closeSdk();

			std::string toString(OpcUa::SecurityMode mode, const std::string& locale = "en");
			std::string toString(OpcUa::MessageSecurityMode mode, const std::string& locale = "en");
			std::string toString(const OpcUa::EndpointDescription& ed);
			std::string toString(const OpcUa::UserIdentityTokenType uitt);
			std::string toString(const OpcUa::ConnectionState state, const std::string& locale = "en");
			std::string toString(const OpcUa::ByteString& value);
      std::string toString(const OpcUa::SecurityTokenRequestType requestType);

			std::string currentTimestamp();
			std::string toLower(const std::string& str);
			bool startsWith(const std::string& str, const std::string& subs);


			boost::posix_time::ptime convert(const std::chrono::system_clock::time_point& from);

			std::chrono::system_clock::time_point convert(const boost::posix_time::ptime& from);

      void initThreadPool(int numberOfThreads = 2);
      std::shared_ptr<ThreadPool> getThreadPool();

			void closeThreadPool();			

			void initCallbackThreadPool(int numberOfThreads = 1);
			std::shared_ptr<ThreadPool> getCallbackThreadPool();
			void closeCallbackThreadPool();

			std::shared_ptr<boost::asio::io_service> claimIoService();
			void releaseIoService(std::shared_ptr<boost::asio::io_service>& service);

      std::string getName(const RequestResponseTypeId typeId);

			// Reads content of the file into string.
			// Returns -1 in case of error, or length of the string in case of success.
			int32_t readFileToString(const std::string& fileName, std::string& content);
			std::string saveStringToFile(const std::string& value, const std::string& fileName);

			std::string saveBytesToFile(const std::vector<uint8_t>& content, const std::string& fileName);
			std::string readBytesFromFile(const std::string& fileName, std::vector<uint8_t>& content);

			void copyFileIfDoesNotExist(const std::string& defaultFile, const std::string& targetFile);

			std::string getHostName();

			std::string toString(NodeClass);

			std::string toString(spdlog::level::level_enum logLevel);
			std::string toString(const SecurityPolicyId id, bool shortForm = true);

			bool fileExists(const std::string& fileName);

			std::string combinePath(const std::string& path1, const std::string& path2);

			std::string getFilesInFolder(const std::string& folderName, std::vector<std::string>& result);

			std::string deleteFile(const std::string& fileName);

			std::vector<std::string> splitString(const std::string& str, const char delimiter, const uint16_t maxTimes = 0xFFFF, const char escapeCharacter = '\0');

			/**
			 * Combine whole string from vector of strings, optionally separating them be delimiter (if it is not 0).
			 */
			std::string combineToString(const std::vector<std::string>& parts, const char delimiter = '\0');

			/**
			 * separate host name part from DNS name (the very first part delimited by dot ('.')).
			 */
			std::string getHostNameFromDns(const std::string dns);

			std::string changeFileExtension(const std::string& fileName, const std::string& newExtension);

			std::string toHex(const std::vector<uint8_t>& bytes);

			bool isCertificateValidationError(OWA::OpcUa::StatusCode statusCode);
			bool hostNameCanBeResolved(const std::string& endpointUrl, uint32_t timeout = 5000);

			bool urlDecode(const std::string& in, std::string& out);

			// Returns folder where log files are written to. Default value is ./data/logs. It is initialized in the initSdk call.
			std::string getLogFolder();

      std::string escapeString(const std::string& str, std::set<char> charsToEscape, const char escapeCharacter);

      std::string unEscapeEstring(const std::string& str, const char escapeCharacter);

			// Removes ending characters from string.
			std::string removeTrailingSymbol(const std::string& str, const std::string& symbols);
		}
	}
}