#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include "opcua/Enums.h"

#include <memory>
#include "opcua/ChannelSecurityToken.h"
#include "opcua/SecurityPolicy.h"

namespace OWA {
  namespace OpcUa {
    class SymmetricCryptoContext {
    public:
			SymmetricCryptoContext();
			SymmetricCryptoContext(ChannelSecurityToken& securityToken, std::shared_ptr<SecurityPolicy> securityPolicy, SecurityMode securityMode);
      void initialize();

      uint32_t getId() { return securityToken.TokenId; }

      SecureByteString PRF(const SecureByteString& secret, const SecureByteString& seed, uint32_t length);

      SecureByteString PRF(const SecureByteString& secret, const SecureByteString& seed, int32_t length, int32_t offset);
      
      ChannelSecurityToken securityToken;

     SecureByteString myNonce;
     SecureByteString mySignKey;
     SecureByteString myEncryptKey;
     SecureByteString myInitializationVector;
     SecureByteString hisNonce;
     SecureByteString hisSignKey;
     SecureByteString hisEncryptKey;
     SecureByteString hisInitializationVector;

     int encryptionKeySize;
     int signatureKeySize;
     int plainTextBlockSize;
     
     int signatureSize;

     SecurityMode securityMode;

    };
  }
}