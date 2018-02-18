#pragma once
#include "opcua/RequestHeader.h"
#include "opcua/ResponseHeader.h"
#include "opcua/ApplicationDescription.h"
#include "opcua/ByteString.h"

namespace OWA {
  namespace OpcUa {
    struct GetEndpointsRequest {
      GetEndpointsRequest(std::string endpointUrl) {
        this->endpointUrl = endpointUrl;
      }
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::GetEndpointsRequest;
      }
      RequestHeader header;
      /** The network address that the Client used to access the Discovery
        Endpoint.
        The Server uses this information for diagnostics and to determine what
        URLs to return in the response.
        The Server should return a suitable default URL if it does not recognize
        the HostName in the URL.
        */
      std::string endpointUrl;
      /**
        List of locales to use.
        Specifies the locale to use when returning human readable strings.
        This parameter is described in 5.4.2.2.
        */
      std::vector<std::string> localeIds;
      /** 
        List of Transport Profile that the returned Endpoints shall support.Part 7
        defines URIs for the Transport Profiles.
        All Endpoints are returned if the list is empty.
        */
      std::vector<std::string> profileUris;

			typedef std::shared_ptr<GetEndpointsRequest> Ptr;
    };
    struct UserTokenPolicy {
      /**
      An identifier for the UserTokenPolicy assigned by the Server.
      The Client specifies this value when it constructs a UserIdentityToken that conforms to the policy.
      This value is only unique within the context of a single Server.
      */
      std::string policyId;

      UserIdentityTokenType tokenType;
      std::string issuedTokenType;
      std::string issuerEndpointUrl;
      std::string securityPolicyUri;
    };

    struct EndpointDescription {
      // The URL for the Endpoint described.
      std::string endpointUrl;

      //The description for the Server that the Endpoint belongs to.
      ApplicationDescription server;

      // The application instance Certificate issued to the Server.
      ByteString serverCertificate;

      //The type of security to apply to the messages.
      // A SecureChannel may have to be created even if the securityMode is NONE.The exact behaviour depends on the mapping used and is described in the Part 6.
      MessageSecurityMode securityMode;

      // The URI for SecurityPolicy to use when securing messages.
      // The set of known URIs and the SecurityPolicies associated with them are defined in Part 7.
      std::string securityPolicyUri;

      // The user identity tokens that the Server will accept.
      // The Client shall pass one of the UserIdentityTokens in the ActivateSession request.The UserTokenPolicy type is described in 7.36.
      std::vector<UserTokenPolicy> userIdentityTokens;

      //The URI of the Transport Profile supported by the Endpoint.
      //Part 7 defines URIs for the Transport Profiles.
      std::string transportProfileUri;

      /**
        A numeric value that indicates how secure the EndpointDescription is compared to other EndpointDescriptions for the same Server.
        A value of 0 indicates that the EndpointDescription is not recommended and is only supported for backward compatibility.
        A higher value indicates better security.
      */
      uint8_t securityLevel;
    };

    struct GetEndpointsResponse {
      ResponseHeader header;
      std::vector<EndpointDescription> endpoints;
    };
  }
}