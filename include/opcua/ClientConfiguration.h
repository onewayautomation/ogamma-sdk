#pragma once 
#include "opcua/CertificateSettings.h"
#include "opcua/ApplicationDescription.h"
#include "opcua/GetEndpoints.h"
#include "opcua/UserIdentityToken.h"
#include "opcua/TransportSettings.h"

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
				return *this;
			}
      std::string endpointUrl;
      std::string productUri;
      std::string applicationUri;
      std::string applicationName;
      std::vector<std::string> discoveryUrls; // Returned by FindServers call. Used to call GetEndpoints.
      std::string localDiscoveryServerUrl; // Used to call FindServers on Local Discovery Server. If empty, the discoveryUrls is used.
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
			uint32_t timeoutChannel;
			uint32_t timeoutDiscovery;
			uint32_t timeoutBrowse;
			uint32_t timeoutRead;
			uint32_t timeoutWrite;
			uint32_t timeoutPublish;
			uint32_t timeoutCall;
			uint32_t timeoutCloseSession;

			// Timeout for other requests for which no default value is defined:
			uint32_t timeoutGeneral;

			// Default connection properties:
      std::string defaultDiscoveryUrl; // = "opc.tcp://localhost:4840";
      std::string defaultServerEndpointUrl; // = "opc.tcp://localhost:48010";

			uint32_t maxReferencesPerNode;
			
			uint32_t parallelPublishRequests;

			TransportSettings transportSettings;

			bool useGetEndpointsHostName;
	  };

    struct ClientConfiguration {
      ClientConfiguration();
			ClientConfiguration(const std::string& endpointUrl, bool createSession = false);
			ClientConfiguration& operator=(const ClientConfiguration& other);

      std::string getDiscoveryUrl(RequestResponseTypeId forRequestType, const std::string& protocol = Protocol::tcp);

			std::string getName();

      ApplicationDescription applicationDescription;
      CertificateSettings certificateSettings;
      SecurityMode securityMode; // If set to invalid, then highest level secured connection mode is used.
      ServerInfo serverInfo;
	    ConnectionSettings connectionSettings;
      Duration requestedSessionTimeout;
      uint32_t maxResponseMessageSize;

      bool createSession; // If true, a session will be created and activated on server. Otherwise, just secure channel is opened.

			// Session name, which is passed in the createSession call to the server.
      std::string sessionName;

			// Name of the configuration. It can be used in GUI applications to identify the connection.
			std::string name;

			std::vector<std::string> localeIds;

			std::shared_ptr<UserIdentityToken> identityToken;
		private:
			void init();
    };
  }
}