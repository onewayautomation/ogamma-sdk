#ifndef owa_ioservice_h
#define owa_ioservice_h
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>

namespace OWA {
	namespace OpcUa {

		class IoService {
		public:
			IoService(int id);
			~IoService();
			
			int getNumberOfUsers();
			void claim();
			void release();
			std::shared_ptr<boost::asio::io_service>& getService();
			void shutdown();
		protected:
			std::shared_ptr<std::thread> serviceThread;
			std::atomic<bool> shuttingDown;
			std::atomic<int> numberOfUsers;
			std::shared_ptr<boost::asio::io_service> service;			
			int id;
		};

		class IoServiceManager {
		public:
			IoServiceManager(int numberOfServices = 1);
			std::shared_ptr<boost::asio::io_service> claimService();
			void releaseService(std::shared_ptr<boost::asio::io_service>& service);
			void shutdown();
			~IoServiceManager();
		protected:
			std::vector<std::shared_ptr<IoService>> services;
		};
	}
}
#endif