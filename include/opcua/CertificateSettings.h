#pragma once
#include <botan/x509self.h>

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
      std::string publicKeyFileName;
      std::string privateKeyFileName;
      bool generate;
      Botan::X509_Cert_Options options;
    };
  }
}