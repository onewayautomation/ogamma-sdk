#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include "opcua/RequestHeader.h"

namespace OWA {
  namespace OpcUa {
    struct OpenSecureChannelRequest {
			OpenSecureChannelRequest();
			~OpenSecureChannelRequest();

			static RequestResponseTypeId getTypeId();

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