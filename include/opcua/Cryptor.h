#pragma once

#include "opcua/SecurityPolicy.h"
#include <memory>
#include "opcua/DataBuffer.h"
#include "opcua/ByteString.h"
#include "opcua/OperationResult.h"
#include "opcua/SecurityPolicy.h"
#include "opcua/CertificateSettings.h"
#include <functional>
#include "opcua/SymmetricCryptoContext.h"

namespace boost { namespace filesystem { class path; } }

namespace OWA {
  namespace OpcUa {
   
    class Cryptor {
    public:
      Cryptor();

      void initialize(SecurityPolicyId id, MessageSecurityMode securityMode, 
        CertificateSettings& myCertificateSettings, X509Certificate& hisCertificate, std::function<std::string()> passwordCallback);
      void setPasswordCallback(std::function<std::string()> f);
      virtual ByteString generateClientNonceForOpenSecureChannelRequest(const std::string& securityPolicyUri);
      virtual ByteString generateClientNonceForCreateSessionRequest(const std::string& securityPolicyUri);

      virtual void finalizeMessage(DataBufferPtr& buffer, uint32_t sequenceHeaderPosition, RequestResponseTypeId typeId, std::shared_ptr<SymmetricCryptoContext>& cryptoContext);
      virtual void asymmetricDecryptAndVerify(DataBufferPtr& buffer);
      virtual void symmetricDecryptAndVerify(DataBufferPtr& buffer, SymmetricCryptoContext& cryptoContext);

      std::shared_ptr<Botan::RandomNumberGenerator> randomNumberGenerator;
      std::shared_ptr<SecurityPolicy> securityPolicy;
      CertificateSettings certificateSettings;
      std::shared_ptr<std::function<std::string()>> privateKeyPasswordCallback;

      MessageSecurityMode messageSecurityMode;
      X509Certificate getMyCertificate();
      X509Certificate getHisCertificate();
      std::string getHisCertificateFingerprint();
      bool validateCertificateThumbprint(const ByteString& thumbPrint);

    protected:
      void save(const std::string& content, const std::string& fileName );
      void save(const std::string& content, const boost::filesystem::path& fileName);
      void save(const std::vector<uint8_t>& content, const boost::filesystem::path& fileName);
      std::string getPrivateKeyPassword();
      void loadCertificate(SecurityPolicyId securityPolicyId, X509Certificate& certificate, std::shared_ptr<Botan::Private_Key>& privateKey);
      std::vector<Botan::byte> encrypt(uint8_t* data, uint32_t length);
    };
  }
}
