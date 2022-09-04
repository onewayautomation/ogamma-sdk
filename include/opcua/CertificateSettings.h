#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include <botan/x509self.h>
#include <map>
#include "opcua/Enums.h"
#include "opcua/OpcUaCertificateStore.h"

namespace OWA {
  namespace OpcUa {
    struct CertificateSettings {
      CertificateSettings();

      std::string rootFolder;
      std::string trusted;
      std::string crl;
      std::string issuers;
      std::string issuersCrl;
      std::map<std::string, std::string> publicKeyFileName;
			std::map<std::string, std::string> privateKeyFileName;
      bool generate;
      Botan::X509_Cert_Options options;
      OpcUaCertificateStore::ValidationRules validationRules;

      // Limits for number of certificates in a chain sent to the server, including CA Certificates and the instance certificate itself.
      // 0 - unlimited; 
      // 1 - only instance certificate;
      // 2 or greater - max chain length.
      uint8_t maxSentChainLength;
    };
  }
}