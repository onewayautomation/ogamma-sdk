#pragma once
#include "opcua/CallbackFunctionDefs.h"
#include "opcua/OperationResult.h"
#include "opcua/ServiceFault.h"
#include "opcua/OpenSecureChannelRequest.h"
#include "opcua/OpenSecureChannelResponse.h"
#include "opcua/FindServersRequest.h"
#include "opcua/FindServersResponse.h"
#include "opcua/GetEndpoints.h"
#include "opcua/CloseSecureChannel.h"
#include "opcua/Session.h"
#include "opcua/Browse.h"
#include "opcua/Attribute.h"
#include "opcua/Subscribe.h"
#include "opcua/Monitor.h"
#include "opcua/Call.h"

#include <future>
#include <chrono>

namespace OWA {
	namespace OpcUa {
		// Identifier to recognize response based on corresponding request. Initially it is request handle.
		typedef uint32_t RequestId;

		struct ClientContext {
			ClientContext(const ClientContext&) = delete;
			ClientContext& operator=(const ClientContext&) = delete;
			~ClientContext();

			ClientContext();
			ClientContext(
				RequestId id, // request handle. For connect/disconnect ands OpenSecureChannel/CloseSecureChannel = 0.
				RequestResponseTypeId type,  // request type as defined by the spec. Exception - connect and service fault request for consistency  
				void* pointerToPromise, // Type of the promise depends on expected response type
				void* pointerToCallbackFunction,
				void* pointerToRequest // - type depends on request type.
			);
			void setTimeout(uint32_t timeout);
			bool isTimedOut();

			RequestId id;
			RequestResponseTypeId type;
			void* promise;
			void* callbackFunction;
			void* request;
			std::shared_ptr<std::shared_future<OperationResult>> sharedFuture; // Required only for connect and disconnect requests
			std::chrono::time_point<std::chrono::system_clock> expireTime;
		};
	}
}
