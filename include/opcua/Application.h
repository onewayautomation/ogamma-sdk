#pragma once
#include "opcua/Connection.h"

namespace OWA {
  namespace OpcUa {
    class Appllication {
    public:
      /**
      * Callbacks which called on Connection/Disconnection to OPC UA Server events
      */
      virtual void onConnect(const boost::any& context, OperationResult result);
      virtual void onCouldNotConnect(const boost::any& context, OperationResult result);
      virtual void onDisconnect(const boost::any& context, OperationResult result);


      virtual void onClientConnect();
      virtual void onClientDisconnect();

      /**
      * Callback method which is called before setting of a result in the future object returned by send method.
      * @param request - original request
      */
      virtual void onResponse(const boost::any& context, std::shared_ptr<FindServersRequest> request, std::shared_ptr<FindServersResponse> response);

      virtual void onRequest(const boost::any& context, std::shared_ptr<FindServersRequest> request, std::shared_ptr<FindServersResponse> response);
    };
  }
}