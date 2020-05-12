#include <opcua/Connection.h>
#include <iostream>

using namespace OWA::OpcUa;
int main (int argc, char** argv)
{
  (void)argc;
  (void)argv;
  const std::string value1 = "Hello World!";
  const std::string value2 = "Hi there!";
  bool succeeded = false;

  // initSdk must be called in order to initialize logging subsystem:
  OWA::OpcUa::Utils::initSdk();

  {
    auto connection = Connection::create("opc.tcp://opcuaserver.com:48010", true);
    if (connection->connect().get().isGood())
    {
      NodeId nodeId("Demo.Static.Scalar.String", 2);
      ReadRequest::Ptr readRequest(new ReadRequest(nodeId));
      auto readResponse = connection->send(readRequest).get();
      if (readResponse->isGood() && readResponse->results.size() == 1 && Utils::isGood(readResponse->results[0].statusCode))
      {
        // We know that data type of the value is String, therefore convert it to string should succeed:
        std::string currentValue = readResponse->results[0].value;
        std::string newValue = (currentValue == value1) ? value2 : value1;
        WriteRequest::Ptr writeRequest(new WriteRequest(WriteValue(nodeId, DataValue(Variant(newValue)))));
        auto writeResponse = connection->send(writeRequest).get();
        if (writeResponse->isGood() && Utils::isGood(writeResponse->results[0]))
        {
          readResponse = connection->send(readRequest).get();
          if (readResponse->isGood() && readResponse->results.size() == 1 && Utils::isGood(readResponse->results[0].statusCode))
          {
            currentValue = readResponse->results[0].value;
            if (currentValue == newValue)
            {
              std::cout << "Wrote value " << currentValue << " to the server!" << std::endl;
              succeeded = true;
            }
          }
        }
      }
    }
  }
  OWA::OpcUa::Utils::closeSdk();
  if (!succeeded)
  {
    std::cout << "Writing new value to the server failed!" << std::endl;
    return -1;
  }
  else
  {
    return 0;
  }
}
