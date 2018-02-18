#pragma once 
#include "opcua/CertificateSettings.h"
#include "opcua/ApplicationDescription.h"
#include "opcua/GetEndpoints.h"

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
      std::string localDiscoveryServerUrl; // Used to call FindServers on local discovery server.
    };
	  
    struct ConnectionSettings {
			ConnectionSettings();

      bool autoConnect;
      bool autoReconnect;

			// Timeouts for various kinds of requests
			uint32_t timeoutConnection;
			uint32_t timeoutDiscovery;
			uint32_t timeoutBrowse;
			uint32_t timeoutRead;
			uint32_t timeoutWrite;
			uint32_t timeoutPublish;
			uint32_t timeoutCall;

			// Timeout for other requests for which no default value is defined:
			uint32_t timeoutGeneral;

			// Default connection properties:
			std::string defaultDisoveryUrl = "opc.tcp://localhost:4840";
			std::string defaultServerEndpointUrl = "opc.tcp://localhost:48010";

			uint32_t maxReferencesPerNode;
			
			uint32_t parallelPublishRequests;
	  };

    struct ClientConfiguration {
      ClientConfiguration();
			ClientConfiguration(const std::string& endpointUrl);

      std::string getDiscoveryUrl(const std::string& protocol = Protocol::tcp);
      ApplicationDescription applicationDescription;
      CertificateSettings certificateSettings;
      SecurityMode securityMode; // If set to invalid, then highest level secured connection mode is used.
      ServerInfo serverInfo;
	    ConnectionSettings connectionSettings;
      Duration requestedSessionTimeout;
      uint32_t maxResponseMessageSize;

      bool createSession; // If true, a session will be created and activated on server. Otherwise, just secure channel is opened.
      std::string sessionName;
			std::vector<std::string> localeIds;
		private:
			void init();
    };
  }
}