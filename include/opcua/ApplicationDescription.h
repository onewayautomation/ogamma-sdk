#pragma once
#include "opcua/LocalizedText.h"
#include "opcua/Enums.h"
#include <vector>

namespace OWA {
  namespace OpcUa {
    struct ApplicationDescription {
      ApplicationDescription() {
        applicationType = ApplicationType::SERVER_0;
      }

      // The globally unique identifier for the application instance.This URI is used as ServerUri in Services if the application is a Server.
      std::string applicationUri;
      
      //The globally unique identifier for the product.
      std::string productUri;
      
      // A localized descriptive name for the application.
      LocalizedText applicationName;
      
      ApplicationType applicationType;
      
      /** A URI that identifies the Gateway Server associated with the discoveryUrls.
        * This value is not specified if the Server can be accessed directly.
        * This field is not used if the applicationType is CLIENT_1.
        */
      std::string gatewayServerUri;
      /**
        A URI that identifies the discovery profile supported by the URLs provided.
        This field is not used if the applicationType is CLIENT_1.
        If this value is not specified then the Endpoints shall support the Discovery Services defined in 5.4.
        Alternate discovery profiles are defined in Part 7.
        */
      std::string discoveryProfileUri;
      /**
        A list of URLs for the discovery Endpoints provided by the application.
        If the applicationType is CLIENT_1, this field shall contain an empty list.
        */
      std::vector<std::string> discoveryUrls;
    };
  }
}