#include <opcua/Connection.h>
#include <iostream>
#include <iomanip>

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
      // In general, all requests and response in this SDK mirror parameters of OPC UA services defined in Part 4 of the OPC UA specifications.

      // Example calling Read and Write services:
      {
        NodeId nodeId("Demo.Static.Scalar.String", 2);
        ReadRequest::Ptr readRequest(new ReadRequest(nodeId));

        // You can read more than one node, by adding node ids to the request:
        ReadValueId rvd(NodeId(2258, 0), (IntegerId)AttributeId::value);
        readRequest->nodesToRead.push_back(rvd);
        // This can be done also without explicitly defining of the ReadValueId type variable:
        readRequest->nodesToRead.push_back(2258);

        // Method send() returns future, and calling get() on it returns shared pointer to the response.
        auto readResponse = connection->send(readRequest).get();

        if (readResponse->isGood() && readResponse->results.size() >= 1 && Utils::isGood(readResponse->results[0].statusCode))
        {
          // We know that data type of the value is String, therefore convert it to string should succeed:
          std::string currentValue = readResponse->results[0].value;
          std::string newValue = (currentValue == value1) ? value2 : value1;
          WriteRequest::Ptr writeRequest(new WriteRequest(WriteValue(nodeId, DataValue(Variant(newValue)))));

          // To write multiple values, just add more elements to the nodesToWrite member of the write request:
          WriteValue wv(NodeId("InvalidNodeId", 2), DataValue(Variant(123)));
          writeRequest->nodesToWrite.push_back(wv);

          auto writeResponse = connection->send(writeRequest).get();
          if (writeResponse->isGood() && Utils::isGood(writeResponse->results[0]))
          {
            readResponse = connection->send(readRequest).get();
            if (readResponse->isGood() && readResponse->results.size() >= 1 && Utils::isGood(readResponse->results[0].statusCode))
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

      // Now try to browse server's address space synchronously:
      {
        BrowseRequest::Ptr browseRequest(new BrowseRequest());
        NodeId nodeIdToBrowse;
        if (!nodeIdToBrowse.parse("ns=2;s=Demo.Massfolder_Static")) // Node id can be parsed from some-how saved serialized string value
        {
          std::cout << "Failed to parse node id" << std::endl;
        }
        else
        {
          BrowseDescription bd(nodeIdToBrowse);
          bd.browseDirection = BrowseDirection::forward;
          bd.includeSubtypes = true;
          bd.referenceTypeId.setNumericIdentifier(Ids::NumericNodeId::HierarchicalReferences);
          browseRequest->nodesToBrowse.push_back(bd);

          // Limit number of references returned, so BrowseNext request can be called too.
          browseRequest->requestedMaxReferencesPerNode = 100;
          int callCounter = 0;

          std::shared_ptr<BrowseResponse> response = connection->send(browseRequest).get();
          callCounter++;
          if (Utils::isGood(response) && response->results.size() == 1)
          {
            BrowseResult& r = response->results[0];

            if (Utils::isGood(r.statusCode))
            {
              ByteString cp = r.continuationPoint;
              std::vector<ReferenceDesciption> allReferences = std::move(r.references);

              while (!cp.empty())
              {
                std::shared_ptr<BrowseNextRequest> browseNextRequest(new BrowseNextRequest());
                browseNextRequest->continuationPoints.push_back(cp);
                BrowseNextResponse::Ptr browseNextResponse = connection->send(browseNextRequest).get();
                callCounter++;
                if (Utils::isGood(browseNextResponse) && browseNextResponse->results.size() == 1 && Utils::isGood(browseNextResponse->results[0].statusCode))
                {
                  cp = browseNextResponse->results[0].continuationPoint;
                  for (auto ri = browseNextResponse->results[0].references.begin(); ri != browseNextResponse->results[0].references.end(); ri++)
                    allReferences.push_back(*ri);
                }
                else
                {
                  cp.clear();
                  std::cout << "Browse Next request failed." << std::endl;
                }
              }

              // How many nodes to print:
              int numberOfNodes = 20;
              std::cout << "Browsing of node [" << nodeIdToBrowse.toString() << "] returned " << allReferences.size() << " references in " <<callCounter << " calls. " << std::endl;
              for (auto iter = allReferences.begin(); iter != allReferences.end() && numberOfNodes > 0; iter++, numberOfNodes--)
              {
                std::cout << "NodeId = [" << iter->nodeId.toString() << "], display name = " << iter->displayName.text;
                std::cout << ", node class = " << Utils::toString(iter->nodeClass) << std::endl;
              }
            }
          }
          else
            std::cout << "Browse request failed." << std::endl;
        }
      }

      // Now try to browse server's address space asynchronously:
      {
        BrowseRequest::Ptr browseRequest(new BrowseRequest());
        NodeId nodeIdToBrowse;
        
        // Node id can be parsed from persisted values serialized to string format:
        if (!nodeIdToBrowse.parse("ns=2;s=Demo.Massfolder_Static")) 
        {
          std::cout << "Failed to parse node id" << std::endl;
        }
        else
        {
          BrowseDescription bd(nodeIdToBrowse);
          bd.browseDirection = BrowseDirection::forward;
          bd.includeSubtypes = true;
          bd.referenceTypeId.setNumericIdentifier(Ids::NumericNodeId::HierarchicalReferences);
          browseRequest->nodesToBrowse.push_back(bd);

          // Limit number of references returned, so BrowseNext request can be called too.
          browseRequest->requestedMaxReferencesPerNode = 100;
          int callCounter = 0;
          std::atomic<bool> browsingComplete = false;
          std::vector<ReferenceDesciption> allReferences;

          connection->send(browseRequest, [&callCounter, &allReferences, connection, &browsingComplete, nodeIdToBrowse](BrowseRequest::Ptr& request, BrowseResponse::Ptr& response) {
            callCounter++;
            if (Utils::isGood(response) && response->results.size() == 1)
            {
              BrowseResult& r = response->results[0];

              if (Utils::isGood(r.statusCode))
              {
                allReferences = std::move(r.references);
                std::shared_ptr<BrowseNextRequest> browseNextRequest(new BrowseNextRequest());
                browseNextRequest->continuationPoints.push_back(r.continuationPoint);
                
                onResponseCallback<BrowseNextRequest, BrowseNextResponse> callbackFunction
                  = [&callCounter, connection, &allReferences, &browsingComplete, &nodeIdToBrowse](BrowseNextRequest::Ptr& bnRequest, BrowseNextResponse::Ptr& browseNextResponse)
                {
                  callCounter++;
                  if (Utils::isGood(browseNextResponse) && browseNextResponse->results.size() == 1 && Utils::isGood(browseNextResponse->results[0].statusCode))
                  {
                    bnRequest->continuationPoints.clear();
                    auto cp = browseNextResponse->results[0].continuationPoint;
                    bnRequest->continuationPoints.push_back(cp);
                    for (auto ri = browseNextResponse->results[0].references.begin(); ri != browseNextResponse->results[0].references.end(); ri++)
                      allReferences.push_back(*ri);
                    if (!cp.empty())
                    {
                      auto callBackHandler = boost::any_cast<onResponseCallback<BrowseNextRequest, BrowseNextResponse>>(bnRequest->context);
                      connection->send(bnRequest, callBackHandler);
                    }
                    else
                    {
                      // Browsing is complete now.
                                      // How many nodes to print:
                      int numberOfNodes = 20;
                      std::cout << "Asynchronous browsing of the node [" << nodeIdToBrowse.toString() << "] returned " << allReferences.size() << " references in " << callCounter << " calls. " << std::endl;
                      for (auto iter = allReferences.begin(); iter != allReferences.end() && numberOfNodes > 0; iter++, numberOfNodes--)
                      {
                        std::cout << "NodeId = [" << iter->nodeId.toString() << "], display name = " << iter->displayName.text;
                        std::cout << ", node class = " << Utils::toString(iter->nodeClass) << std::endl;
                      }
                      browsingComplete = true;
                    }
                  }
                  else
                  {
                    std::cout << "Browse Next request failed." << std::endl;
                    browsingComplete = true;
                  }

                  return true;
                };

                // Once defined callback function can be made accessible when it is called, by saving copy in the request context:
                browseNextRequest->context = boost::any(callbackFunction);

                connection->send(browseNextRequest, callbackFunction);
              }
            }
            else
              std::cout << "Browse request failed." << std::endl;

            return true;
          });

          // Will block here until browsing is complete. 
          while (!browsingComplete) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
          }
        }
      }

      // Now create subscription and monitored items, and get some data changes from the server:
      {
        std::shared_ptr<CreateSubscriptionRequest> csr(new CreateSubscriptionRequest());
        csr->requestedPublishingInterval = 1000;
        Duration samplingRate = 500;
        uint32_t queueSize = 10;

        CreateMonitoredItemsRequest::Ptr createMonItemsRequest;

        // This callback function will be called every time when Publish response is received.
        NotificationObserver notificationCallback = [&createMonItemsRequest](NotificationMessage& notificationMessage)
        {
          std::cout << "Received notification message with sequence number " << notificationMessage.sequenceNumber << std::endl;

          for (auto iter = notificationMessage.notificationData.begin(); iter != notificationMessage.notificationData.end(); iter++)
          {
            DataChangeNotification* dc = dynamic_cast<DataChangeNotification*>(iter->get());
            if (dc != 0)
            {
              for (auto mi = dc->monitoredItems.begin(); mi != dc->monitoredItems.end(); mi++)
              {
                std::cout << "\tClient Handle = " << mi->clientHandle;

                // Having create Monitored Items request, possible to get NodeId. Note this works only when client handles start from 0 and incremented by 1.
                if (createMonItemsRequest && createMonItemsRequest->itemsToCreate.size() > mi->clientHandle)
                {
                  std::cout << ", NodeId = " << createMonItemsRequest->itemsToCreate[mi->clientHandle].itemToMonitor.nodeId.toString();
                }

                if (Utils::isGood(mi->dataValue.statusCode))
                {
                  std::cout << ", source timestamp = " << mi->dataValue.sourceTimestamp.toString(true) <<
                    ", value = " << mi->dataValue.value.toString() << std::endl;
                } 
                else
                {
                  std::cout << ", server timestamp = " << mi->dataValue.serverTimestamp.toString(true) << ", status is bad (" << Utils::toString(mi->dataValue.statusCode) << ")" << std::endl;
                }
              }
            }
          }
        };

        // If the third parameter in the send() method, where Create Subscription request is sent, is set to false, it means that SDK will actually call Create Subscription request and returned response will have that call result. 
        // If not connected to the server, it would fail. 
        // If third parameter is set to true, then this call would succeed, even if currently not connected to the server. SDK will create subscription when connection is established.
        // The same can be done with Create Monitored Items.
        // This way you can create subscriptions and monitored items once at your application start, and do not think about re-creating them after connection failures or re-connections
        // To show you how it works, lets disconnect before creating subscription and monitored items.
        
        connection->disconnect().get();

        auto createSubscriptionResponse = connection->send(csr, notificationCallback, true).get();

        if (createSubscriptionResponse->isGood())
        {
          createMonItemsRequest.reset(new CreateMonitoredItemsRequest());

          createMonItemsRequest->subscriptionId = createSubscriptionResponse->subscriptionId;

          MonitoredItemCreateRequest micr(NodeId(std::string("Demo.Dynamic.Scalar.String"), 2), samplingRate, queueSize);
          
          // Clint Handle allows to figure-out what monitored item value reported by the server corresponds to.
          // If not defined, SDK will assign values automatically, starting from 1 and incrementing by 1.
          micr.monitoringParameters.clientHandle = 0;
          
          createMonItemsRequest->itemsToCreate.push_back(micr);

          createMonItemsRequest->itemsToCreate.push_back(MonitoredItemCreateRequest(NodeId(std::string("Demo.Dynamic.Scalar.XmlElement"), 2), samplingRate, queueSize));
          // Server Status Node. Complex type, but anyway should not cause decoding error. Value needs to be decoded by the application.
          createMonItemsRequest->itemsToCreate.push_back(MonitoredItemCreateRequest(NodeId(2256), samplingRate, queueSize));

          for (int index = 0; index < 10; index++)
          {
            std::stringstream s;
            s << "Demo.Massfolder_Dynamic.Variable" << std::dec << std::setw(4) << std::setfill('0') << index;
            createMonItemsRequest->itemsToCreate.push_back(MonitoredItemCreateRequest(NodeId(s.str(), 2), samplingRate, queueSize));
          }

          auto response = connection->send(createMonItemsRequest, true).get();
          if (!response->isGood())
          {
            std::cout << "Create Monitored Items call failed, error = " << Utils::toString(response->header.serviceResult);
          }
          else
          {
            // You can check revised status codes, get monitoring parameters revised by the server, etc.
            // response->results[0].revisedSamplingInterval;
          }
        }

        // and now connect without blocking:
        connection->connect();

        std::this_thread::sleep_for(std::chrono::milliseconds(15000)); // Let some data changes to be reported
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
