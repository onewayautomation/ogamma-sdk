#pragma once 
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include "opcua/CertificateSettings.h"
#include "opcua/ApplicationDescription.h"
#include "opcua/GetEndpoints.h"
#include "opcua/UserIdentityToken.h"
#include "opcua/TransportSettings.h"
#include "opcua/Call.h"

namespace OWA {
  namespace OpcUa {
    struct ServerInfo {
      ServerInfo();
			ServerInfo& operator=(const EndpointDescription& ed) {
				*this = ed.server;
				this->endpointUrl = ed.endpointUrl;
				return *this;
			}
			ServerInfo& operator=(const ApplicationDescription& ad) {
				this->applicationName = ad.applicationName.text;
				this->applicationUri = ad.applicationUri;
				this->productUri = ad.productUri;
				this->discoveryUrls = ad.discoveryUrls;
			  this->gatewayServerUri = ad.gatewayServerUri;
				return *this;
			}
			operator ApplicationDescription() const;

      std::string endpointUrl;
			
			
			/* The URL used when the object was created. This might be IP address, or host name of the Docker host machine
			   It allows to create initial connection to call FindServers and GetEndpoints */
			std::string originalEndpointUrl;

			/* Cached endpoint URL. */
			std::string lastSuccessfulEndpointUrl;

      std::string productUri;
      std::string applicationUri;
      std::string applicationName;
      std::vector<std::string> discoveryUrls; // Returned by FindServers call. Used to call GetEndpoints.
      std::string localDiscoveryServerUrl; // Used to call FindServers on Local Discovery Server. If empty, the discoveryUrls is used.
			std::string gatewayServerUri;
    };
	  
    struct ConnectionSettings {
			ConnectionSettings();

      bool autoConnect;
      bool autoReconnect;

			// When unexpected disconnections occurs, waits for this delay before starting first reconnection attempt.
			int delayBeforeReconnect;

			// Interval between  consequential reconnection attempts.
			uint32_t reconnectInterval;

			// Interval to read server state (connection watchdog).
			uint32_t readServerStateInterval;
			uint32_t timeoutReadServerState;
			uint32_t pendingRequestsProcessingInterval;

			// Timeouts for various kinds of requests
			uint32_t timeoutConnection;
			uint32_t timeoutHostNameResolution;
			uint32_t timeoutChannel;
			uint32_t timeoutDiscovery;
			uint32_t timeoutBrowse;
			uint32_t timeoutRead;
			uint32_t timeoutWrite;
			uint32_t timeoutPublish;
			uint32_t timeoutCall;
			
			uint32_t timeoutCreateSession;
			uint32_t timeoutActivateSession; //NEW

			uint32_t timeoutCreateSubscription; //NEW
			uint32_t timeoutCreateMonitoredItems; //NEW

			uint32_t timeoutCloseSession;

			// Timeout for other requests for which no default value is defined:
			uint32_t timeoutGeneral;

			// Default connection properties:
      std::string defaultDiscoveryUrl; // = "opc.tcp://localhost:4840";
      std::string defaultServerEndpointUrl; // = "opc.tcp://localhost:48010";

			uint32_t maxReferencesPerNode;
			
			uint32_t parallelPublishRequests;

			TransportSettings transportSettings;

			// If this option is true, then in case of connection problems it would try to use host name which was used in GetEndpoints or FindServers call.
			bool useGetEndpointsHostName;

			// Total number of browse references
			uint32_t browseTotalReferences;
			uint32_t browseTotalTimeout;

			// Flag telling if stale connection / disconnection transactions should be monitored. 
			bool monitorStaleTransactions;
	  };

    struct ClientConfiguration {
      ClientConfiguration();
			ClientConfiguration(const std::string& endpointUrl, bool createSession = false);
			ClientConfiguration& operator=(const ClientConfiguration& other);

			// Get URL which is used for validation of the connection ( particularly, sent in HelloMessage and CreateSession, 
			// and used to validate URL in certificate. 
			std::string getDiscoveryUrl(RequestResponseTypeId forRequestType, const std::string& protocol = Protocol::tcp);
			
			std::string getUrlForProtocol(const std::vector<std::string>& urls, const std::string& protocol = Protocol::tcp);

			// Resets cached values (server certificate, obtained from the server endpoint URLs, etc,), to start connection process from scratch.
			// Can be called after failing connection attempts.
			void Reset();

			std::string getName() const;

      ApplicationDescription applicationDescription;
      CertificateSettings certificateSettings;
      SecurityMode securityMode; // If set to invalid, then highest level secured connection mode is used.
      ServerInfo serverInfo;
	    ConnectionSettings connectionSettings;
      Duration requestedSessionTimeout;
      uint32_t maxResponseMessageSize;
			uint32_t maxMonitoredItemsPerSubscription;
			
			ApplicationDescription	serverDescription; // As returned in the findServers response.
			EndpointDescription			serverEndpointDescription; // As returned by the server in the GetEndpoints request, and selected for connection.

      bool createSession; // If true, a session will be created and activated on server. Otherwise, just secure channel is opened.

			// Session name, which is passed in the createSession call to the server.
      std::string sessionName;

			// Name of the configuration. It can be used in GUI applications to identify the connection.
			std::string name;

			std::vector<std::string> localeIds;

			std::shared_ptr<UserIdentityToken> identityToken;
			
			/* If this option is true, then clientCertificate field in the Create Session call will be empty if the connection security mode is none */
			bool doNotSendCertInCreateSessionIfSecModeNone;

			/* DNS Resolver Map, used to substitute host name returned by FindServers or GetEndpoints response. */
			std::map<std::string, std::string> dnsMap;

			/* EndpointURL map. Used to substitute whole endpoint URLm including protocol and port number.*/
			std::map<std::string, std::string> urlMap;

			// If true, after ActivateSession call it will read type information as defined by OPC UA spec. version 1.04 and initialize NamespaceManager with that.
			// (Under node Types/Data Types/OPC Binary)
			bool readXmlTypeDictionaryOnConnect;

			// If true, after Activate Session call it will read data type definitions under node Types / Data types / Base Data Type / Structure.
			bool readTypeDefinitionsOnConnect;

			// Maximum depth of subtypes allowed.
			uint16_t maxSubtypeDepth;

			// If true, some errors occurred during reading of data type definitions, will be ignored.
			bool ignoreTypeDefinitionReadErrors;
			
			uint32_t timeoutGetServerInfo; // Timeout to read namespace array, initialize NamespaceManager after ActivateSession call. 

			// List of methods to call after ActivateSession - to initialize the server.
			std::vector<CallMethodRequest> methodsToCall;

		private:
			void init();
    };
  }
}