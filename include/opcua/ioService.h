#ifndef owa_ioservice_h
#define owa_ioservice_h

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
		protected:
			std::vector<std::shared_ptr<IoService>> services;
		};
	}
}
#endif