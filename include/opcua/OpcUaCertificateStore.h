#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include <botan/x509cert.h>
#include <botan/x509self.h>
#include <botan/rng.h>
#include <botan/rsa.h>
#include <botan/x509_crl.h>
#include <botan/pem.h>
#include <botan/pkcs8.h>

#include <string>
#include <vector>
#include <functional>

#include <opcua/OperationResult.h>
#include "opcua/Enums.h"
#include "opcua/ApplicationDescription.h"

namespace OWA
{
  namespace OpcUa 
  {
    class OpcUaCertificateStore {
    public:
      struct ValidationRules {
        ValidationRules() 
        {
          suppressSecurityPolicyCheck = true;
          suppressValidityPeriodError = false;
          suppressHostNameMisMatch = false;
          suppressCertificateUsageError = false;
          suppressFindRevocationListError = false;
          checkOnlyHostNamePartInDns = true;
          allowHostNameOnlyInUri = true;
          acceptAnyCertificate = false;
        }
        bool suppressSecurityPolicyCheck;
        bool suppressValidityPeriodError;
        bool suppressHostNameMisMatch;
        bool suppressCertificateUsageError;
        bool suppressFindRevocationListError;
        bool checkOnlyHostNamePartInDns;
        /**
         * If this option is true, then URI in the certificate can match with uri in the app description where only host name is used.
         */
        bool allowHostNameOnlyInUri;

        // For testing only, should not be set to true in production. 
        bool acceptAnyCertificate;
      };

      OpcUaCertificateStore();
      OperationResult Init(std::function<std::string()> getPasswordCallback, 
        std::function<void(Botan::X509_Cert_Options&)> getCaCertOptions, std::function<void(Botan::X509_Cert_Options&)> getInstanceCertOptions);

      virtual ~OpcUaCertificateStore() {};

      virtual OperationResult buildCertificatesChain(const Botan::X509_Certificate& certificate, 
        std::vector<std::shared_ptr<Botan::X509_Certificate>>& certificatesChain, bool& isTrusted);

      virtual OperationResult findParentCertificate(const Botan::X509_Certificate& certificate, 
        const std::vector<std::shared_ptr<Botan::X509_Certificate>>& certificates,
        std::shared_ptr<Botan::X509_Certificate>& parentCertificate );

      virtual OperationResult addTrustedCertificate(const Botan::X509_Certificate& certificate) = 0;
      virtual OperationResult getTrustedCertificates(std::vector<std::shared_ptr<Botan::X509_Certificate>>& certificates) = 0;

      virtual OperationResult getTrustedCertificate(const std::string& fingerprint, std::shared_ptr<Botan::X509_Certificate>& certificate) = 0;
      virtual OperationResult getIssuerCertificate(const std::string& fingerprint, std::shared_ptr<Botan::X509_Certificate>& certificate) = 0;
      virtual OperationResult getRejectedCertificate(const std::string& fingerprint, std::shared_ptr<Botan::X509_Certificate>& certificate) = 0;
      
      virtual OperationResult addIssuerCertificate(const Botan::X509_Certificate& certificate) = 0;
      virtual OperationResult getIssuerCertificates(std::vector<std::shared_ptr<Botan::X509_Certificate>>& certificates) = 0;

      virtual OperationResult getRejectedCertificates(std::vector<std::shared_ptr<Botan::X509_Certificate>>& certificates) = 0;

      virtual OperationResult saveCaCertificate(const Botan::X509_Certificate& certificate) = 0;
      virtual OperationResult getCaCertificate(std::shared_ptr<Botan::X509_Certificate>& certificate) = 0;

      virtual OperationResult saveCaCrl(const std::string& policyId, const Botan::X509_CRL& certificate) = 0;
      virtual OperationResult getCaCrl(const std::string& policyId, std::shared_ptr<Botan::X509_CRL>& certificate) = 0;

      virtual OperationResult getCrls(std::vector<std::shared_ptr<Botan::X509_CRL>>& crls) = 0;
      virtual OperationResult addCrl(const Botan::X509_Certificate& certificate, const Botan::X509_CRL& crl) = 0;
      virtual OperationResult getCrl(const Botan::X509_Certificate& certificate, std::shared_ptr<Botan::X509_CRL>& crl) = 0;

      virtual OperationResult saveCaPrivateKey(const Botan::Private_Key& key) = 0;
      virtual OperationResult getCaPrivateKey(std::shared_ptr<Botan::Private_Key>& key) = 0;

      virtual OperationResult saveInstanceCertificate(const std::string& policyId, const Botan::X509_Certificate& certificate) = 0;

      /**
       * return: if the certificate does not exist, creates it. 
       */
      virtual OperationResult getInstanceCertificate(const std::string& policyId, std::shared_ptr<Botan::X509_Certificate>& certificate) = 0;
      
      virtual OperationResult saveInstancePrivateKey(const std::string& policyId, const Botan::Private_Key& key) = 0;

      /**
       * return: if the certificate does not exist, creates it.
       */
      virtual OperationResult getInstancePrivateKey(const std::string& policyId, std::shared_ptr<Botan::Private_Key>& key) = 0;

      virtual OperationResult validateCertificate(
        const Botan::X509_Certificate& certificate,
        const ApplicationDescription& otherApplicationDescription,
        const ValidationRules& validationRules,
        const ApplicationType validatingAsAppType = ApplicationType::SERVER_0, const std::string& endpointUrl = "");

      virtual OperationResult saveAsRejected(const std::shared_ptr<Botan::X509_Certificate>& certificate) = 0;

      virtual OperationResult generateCaCertificate(std::shared_ptr<Botan::X509_Certificate>& certificate, std::shared_ptr<Botan::Private_Key>& key);
      virtual OperationResult generateCrl(const Botan::X509_Certificate& caCertificate, const Botan::Private_Key& caKey,
        std::shared_ptr<Botan::X509_CRL>& crl);

      virtual OperationResult generateInstanceCertificate(const std::string& policyId, OWA::OpcUa::ApplicationType appType, 
        Botan::X509_Certificate& caCertificate, Botan::Private_Key& caKey,
        std::shared_ptr<Botan::X509_Certificate>& certificate, std::shared_ptr<Botan::Private_Key>& key);
      
             virtual std::map<std::string, std::string> getCertificateFields(const Botan::X509_Certificate& certificate);

      virtual void getCrlFields(const Botan::X509_CRL& crl, std::map<std::string, std::string>& fields, std::vector<std::map<std::string, std::string>>& entries);

      virtual OperationResult getCrl(const std::string& fingerPrint, std::shared_ptr<Botan::X509_CRL>& crl) = 0;

      virtual OperationResult deleteTrustedCertificate(const std::string& fingerPrint) = 0;
      virtual OperationResult deleteIssuerCertificate(const std::string& fingerPrint) = 0;
      virtual OperationResult deleteRejectedCertificate(const std::string& fingerPrint) = 0;
      virtual OperationResult deleteCrl(const std::string& fingerPrint) = 0;

      virtual OperationResult clearTrustedCerts() = 0;
      virtual OperationResult clearIssuerCerts() = 0;
      std::shared_ptr<Botan::RandomNumberGenerator> getRandomNumberGenerator();

      bool isParent(const Botan::X509_Certificate& certificate, const Botan::X509_Certificate& possibleParent);
      static bool isSignedBy(const Botan::X509_CRL& crl, const Botan::X509_Certificate& caCert);

      const std::string defaultPolicyId = "Basic256Sha256";
      
      static OperationResult decodeFromDerBuffer(const std::vector<uint8_t> bytes, std::vector<std::shared_ptr<Botan::X509_Certificate>>& certificates);

      static std::string getCommonName(const Botan::X509_Certificate& cert);

     protected:
      std::shared_ptr<Botan::RandomNumberGenerator> randomNumberGenerator;
      std::function<std::string()> getPasswordCallback;
      std::function<void(Botan::X509_Cert_Options&)> getCaCertOptions;
      std::function<void(Botan::X509_Cert_Options&)> getInstanceCertOptions;

    };
  }
}

