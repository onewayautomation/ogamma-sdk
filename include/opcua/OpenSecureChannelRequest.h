#pragma once
#include "opcua/RequestHeader.h"

namespace OWA {
  namespace OpcUa {
    struct OpenSecureChannelRequest {
      OpenSecureChannelRequest() {
        requestType = SecurityTokenRequestType::Issue;
        securityMode = MessageSecurityMode::None;
        securityPolicyUri = "http://opcfoundation.org/UA/SecurityPolicy#None";
        requestedLifeTime = 36000000;
      }
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::OpenSecureChannelRequest;
      }
      RequestHeader header;
      ByteString  clientCertificate; //Der encoded certificate
      SecurityTokenRequestType requestType;
      ByteString  secureChannelId;
      MessageSecurityMode securityMode;
      std::string securityPolicyUri;
      ByteString clientNonce;
      uint32_t requestedLifeTime;
      ByteString receiverCertificateThumbPrint; //Thumb print of the server certificate

			typedef std::shared_ptr<OpenSecureChannelRequest> Ptr;
    };
  }
}