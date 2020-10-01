#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include <memory>
#include <functional>
#include "opcua/OperationResult.h"
#include "opcua/Enums.h"
#include "opcua/Subscribe.h"

namespace OWA { namespace OpcUa {
  /**
  * Defines type of callback function used to notify about connect / disconnect events.
  */
  typedef std::function <void(const OperationResult& result)> generalCallback;

  typedef std::function <void(const std::string& endpointUrl, ConnectionState state, const OperationResult& result)> connectionStateChangeCallback;

  /**
  * The onResponseCallback is usually used to make a callback when a response received for the request.
  * Note that it has a request too as an argument, not only the response.
  * It means that the request object will exist until a response is received.
  * Although it would be enough if the request exists until encoding is done.
  * Actually encoding is done before returning from the send method call, so after method call the request could be destroyed.
  * But sometimes it is convenient to have a request available in order to interpret response.
  * For example, ReadRequest stores NodeIds to read, and the response has only data values, not nodeIds.
  * So it is convenient to have a request to figure out what read data value is for what NodeId.
  * For that reason, a Request is kept until callback is done.
  */
  template<typename RequestType, typename ResponseType>
  using onResponseCallback = std::function<bool(std::shared_ptr<RequestType>&, std::shared_ptr<ResponseType>&)>;

  template<typename ResponseType> using onResponseReceived = std::function<bool(std::shared_ptr<ResponseType>&)>;
	
	typedef std::function<void(NotificationMessage& notificationMessage)> NotificationObserver;

  /**
  * onRequestCallback is called when a request is received.
  */
  template<typename RequestType> using onRequestCallback = std::function<bool(std::shared_ptr<RequestType>&)>;
} }