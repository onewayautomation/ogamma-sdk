#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include "opcua/OpcUaCertificateStore.h"
#include <functional>

namespace OWA
{
  namespace OpcUa
  {
    class OpcUaFileCertificateStore : public OpcUaCertificateStore
    {
    public:
      OpcUaFileCertificateStore();
      virtual ~OpcUaFileCertificateStore();

      OperationResult Init(const std::string& pkiRootFolder, std::function<std::string()> getPasswordCallback,
        std::function<void(Botan::X509_Cert_Options&)> getCaCertOptions, std::function<void(Botan::X509_Cert_Options&)> getInstanceCertOptions);

      virtual OperationResult addTrustedCertificate(const Botan::X509_Certificate& certificate);
      virtual OperationResult getTrustedCertificates(std::vector<std::shared_ptr<Botan::X509_Certificate>>& certificates);

      virtual OperationResult addIssuerCertificate(const Botan::X509_Certificate& certificate);
      virtual OperationResult getIssuerCertificates(std::vector<std::shared_ptr<Botan::X509_Certificate>>& certificates);

      virtual OperationResult getRejectedCertificates(std::vector<std::shared_ptr<Botan::X509_Certificate>>& certificates);

      virtual OperationResult saveCaCertificate(const Botan::X509_Certificate& certificate);
      virtual OperationResult getCaCertificate(std::shared_ptr<Botan::X509_Certificate>& certificate);
      virtual OperationResult saveCaPrivateKey(const Botan::Private_Key& key);
      virtual OperationResult getCaPrivateKey(std::shared_ptr<Botan::Private_Key>& key);

      virtual OperationResult saveCaCrl(const std::string& policyId, const Botan::X509_CRL& certificate);
      virtual OperationResult getCaCrl(const std::string& policyId, std::shared_ptr<Botan::X509_CRL>& certificate);

      virtual OperationResult getCrls(std::vector<std::shared_ptr<Botan::X509_CRL>>& crls);
      virtual OperationResult addCrl(const Botan::X509_Certificate& certificate, const Botan::X509_CRL& crl);
      virtual OperationResult getCrl(const Botan::X509_Certificate& certificate, std::shared_ptr<Botan::X509_CRL>& crl);

      OperationResult getCrl(const std::string& fingerPrint, std::shared_ptr<Botan::X509_CRL>& crl) override;

      virtual OperationResult saveInstanceCertificate(const std::string& policyId, const Botan::X509_Certificate& certificate);
      virtual OperationResult getInstanceCertificate(const std::string& policyId, std::shared_ptr<Botan::X509_Certificate>& certificate);
      virtual OperationResult saveInstancePrivateKey(const std::string& policyId, const Botan::Private_Key& key);
      virtual OperationResult getInstancePrivateKey(const std::string& policyId, std::shared_ptr<Botan::Private_Key>& key);
    
      virtual OperationResult saveAsRejected(const std::shared_ptr<Botan::X509_Certificate>& certificate);

      OperationResult deleteTrustedCertificate(const std::string& fingerPrint);
      OperationResult deleteIssuerCertificate(const std::string& fingerPrint);
      OperationResult deleteRejectedCertificate(const std::string& fingerPrint);
      OperationResult deleteCrlForCertificate(const Botan::X509_Certificate & certificate);
      OperationResult deleteCrl(const std::string& fingerPrint);

      virtual OperationResult clearTrustedCerts();
      virtual OperationResult clearIssuerCerts();

      virtual OperationResult getTrustedCertificate(const std::string& fingerprint, std::shared_ptr<Botan::X509_Certificate>& certificate) override;
      virtual OperationResult getIssuerCertificate(const std::string& fingerprint, std::shared_ptr<Botan::X509_Certificate>& certificate) override;
      virtual OperationResult getRejectedCertificate(const std::string& fingerprint, std::shared_ptr<Botan::X509_Certificate>& certificate) override;
    protected:

      std::string getInstanceCertificateFileName(const std::string& policyId);
      std::string getInstancePrivateKeyfileName(const std::string& policyId);

      std::string getCaCertificateFileName(const std::string& policyId);
      std::string getCaPrivateKeyFileName(const std::string& policyId);
      std::string getCaCrlFileName(const std::string& policyId);
      std::string getTrustedCertificatesFolderName();
      std::string getIssuerCertificatesFolderName();
      std::string getRejectedCertificatesFolderName();
      std::string getCrlFolderName();
      std::string getCrlFileName(const Botan::X509_Certificate& certificate);
      
      
      std::string pkiRootFolder;

      const std::string trustedCertsFolderName = "trusted/certs";
      const std::string trustedCrlsFolderName = "trusted/crl";
      const std::string issuersFolderName = "issuers/certs";
      const std::string issuersCrlsFolderName = "issuers/crl";
      const std::string rejectedCertsFolderName = "rejected";

    };
  }
}