# 1WaOpcUaSdk
C++ OPC UA SDK from One-Way Automation - public redistributables.

## Summary
This SDK is intended to extremely simplify creation of OPC UA Client and Server Applications in modern C++. The plan is to implement OPC UA communication stack and provide high level classes to asynchronously send, receive and handle OPC UA requests and responses, and have built-in support for standard OPC UA Server or Client features (example for server side - managing of endpoints, namespaces, sessions, interface for plug-ins to collect data from external data sources, etc). Primary target and narrow specialization of this SDK are high performance OPC UA applications running in PC or cloud environment. It is expected that it will be resource unpretentious and run in lower end hardware such as Raspberry PI though, but narrow specialization will be to satisfy requirements for enterprise scale applications.

Includes header files and pre-built binaries for Visual Studio 2019.

**Binaries for other targets can be created up on request - please feel free to create an issue!**

## Key differentiators
The key differentiators of this SDK from other existing in the market C++ SDKs will be:

 - Designed following "Low Code" philosophy: applications using it would need to write minial code. Example of writing value to single node:
````C++    
    WriteRequest::Ptr req(new WriteRequest(WriteValue (NodeId("Demo.Static.Scalar.UInt32", 2), DataValue((uint32_t) 123))));
    auto writeResult = connection->send(req).get();
    if (writeResult->isGood())
````    
 - Written in modern C++ (11 or higher);
 - Leverages existing open source libraries (boost for asynchronous communication, Botan for cryptography, ODB for Object-Relational Mapping)  to shorten time to market.
 - Cloud ready - microservice-style applications can be created by using of this SDK, which then should be able to be deployed and run on such environments as Azure Service Fabric and Cloud Foundry. 
- Foundation for IoT connectivity applications to deliver data and object model from various factory level data sources (OPC UA Servers) to the Cloud and then provide access to that **data and model** from the Internet via OPC UA interface.
- Does not depend on OPC Foundation's ANSI C Stack, has its own stack implementation.
- Asynchronous interface (which can also be used as synchronous). For client side, the interface uses std::future to return results and optionally possible to handle result via callback defined as std::function. Callbacks are made from dedicated for that threads, so they do not affect other threads such as io service threads.This simplifies using the SDK for GUI applications for example.
- Easy extensibility to add new encoding or transport protocols. Supported encoding/transport combinations could be:
  - binary / opc.tcp
  - binary / opc.amqp
  - json / opc.amqp
  - json / https
- Two way communication - once an OPC UA session is established, both sides of the session can send/receive requests and responses (so it is different from "reversible" connection using ReverseHelloMessage, which still one-way mode). Use cases where this feature can be beneficial:
  - Can be used as true Machine To Machine communication media;
  - Can be used to build cloud relay without opening single port in the firewall too.
  - Can simplify communication between redundancy set members.
- For Client side, automatic re-connection with automatic creation of subscriptions and adding monitored items. 
- Persistent storage to save/load configuration parameters and some current state parameters in both client side and server side:
  - Client: 
    - Namespace array to keep NodeIds static even if server side namespace array is changed;
    - Subscriptions and monitored items. No server side handles are exposed - instead static client handles defined by the application are used, which simplifies mapping of received data changes to the right recipient after re-connections for example.
    - Maintains cache of server's address space, so it can be browsed in offline mode when target server is not available;
  - Server:
    - Address space stored in data base. This removes restrictions for the number of nodes in address space by RAM size. As a result, it makes easy to create for example horizontally scale-able OPC UA Aggregating Server, which can combine address space of hundreds and thousands of factory level OPC UA Servers into common address space in the Cloud, accessible via single endpoint. The same endpoint can also be used to access historical and real time data, with high throughput (millions of data changes per second, utilizing fleet of instances if required).
    - Sampled data values queue stored in database - storing it allows implementation of coming new feature in OPC UA "durable subscriptions" - where even long disconnections do not cause data loss.
    - Sessions and Monitored Items - makes easy to implement "transparent redundancy", where after re-connecting to the other instance, the client does not need to create subscriptions and monitored items again. This is also directly related to support of horizontal scale-ability.
## Current status of the project
Source code is stored in private repository. Technical preview version is ready for the client side.
### Implemented features for client side:
- Establishing TCP connection and initial handshake (OPC UA Hello, Acknowledge and Error messages);
- Implemented OPC UA Services:
  - OpenSecureChannel
  - FindServers
  - GetEndpoints
  - CreateSession
  - ActivateSession
  - CloseSession
  - Call
  - Read
  - Write
  - HistoryRead  
  - Browse
  - BrowseNext
  - CreateSubscription
  - DeleteSubscriptions
  - CreateMonitoredItems (for Data Changes only, no Alarms/Events)
  - DeleteMonitoredItems;
  - Publish
  - CloseSecureChannel
- Communication in secured mode and infrastructure to support it:
  - Generating self-signed root certificate and application instance certificate signed by it;
  - Creation of secure channel in secured mode (sign and encryption). Currently only security policy Basic256Sha256 is supported.
- OPC Binary encoding
  - Implemented for most data types, which are required to send requests and receive responses for functionality listed above.
  - Encoding and Decoding of complex data type values can be handled by the user application, so no need to modify the SDK to support complex data types.

### Next versions
TBD.

## License and Copyright
Copyright 2018-Present, One-Way Aautomation Inc.

For open source projects, research, education and non-commercial organizations, as well as for personal usage by developers free binary license is available.

For detailed licensing information please refer [Developer's Guide, section on licensing](https://onewayautomation.com/opcua-sdk-docs/html/introduction.html#licensing).

## How to use

- Open windows command line console and navigate to the root folder where source code repositories will be located.
- Clone the project: ``git clone https://github.com/onewayautomation/ogamma-sdk.git``
- Navigate to the folder with OPC UA SDK: ``cd 1WaOpcUaSdk``
- Pull and build dependencies by running of the script ``install-opcuasdk-dependencies.cmd``. Note that is takes significant time to build them.
- SDK binary library files for Visual Studio 2019 are available to download from https://onewayautomation.com/opcua-binaries/ogamma-sdk-1.0.0.zip  Content of zip file should be extracted into sub-folder ``lib`` of the SDK source code repository.

Distributables of the SDK include sample **HelloWorld** project located under folder **examples**, which can be used as a base for your first project.

More details on prerequisites and how to use the SDK can be found in the Developer's Guide for the SDK (currently under development, avalable here: [OPC UA C++ SDK Developer's Guide](https://onewayautomation.com/opcua-sdk-docs/html/index.html) )

Please [contacts us](https://onewayautomation.com/index.php/contact-us) if you have questions, issues on the SDK or want to purchase licenses for its commercial use.
