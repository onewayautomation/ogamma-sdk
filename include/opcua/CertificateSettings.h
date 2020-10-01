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
    };
  }
}