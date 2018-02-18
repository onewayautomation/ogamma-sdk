#pragma once
#include <botan/x509cert.h>
#include <botan/block_cipher.h>
#include <botan/pubkey.h>
#include "opcua/Enums.h"

#include <memory>
#include <map>

#include "opcua/ByteString.h"
#include "opcua/OperationResult.h"

namespace OWA {
  namespace OpcUa {
    typedef std::shared_ptr<Botan::X509_Certificate> X509Certificate;

    class SecurityPolicy {
    public:
      SecurityPolicy();

      void initialize(
        SecurityPolicyId id, 
        X509Certificate myCerticate, 
        std::shared_ptr<Botan::Private_Key> myPrivateKey,
        X509Certificate hisCerticate);

      static std::string getUriForId(SecurityPolicyId id);
      static SecurityPolicyId getIdForUri(const std::string securityPolicyUri);
      static int32_t getDerivedSignatureKeyLength(SecurityPolicyId id);
      static int32_t getMinAsymmetricKeyLength(SecurityPolicyId id);
      static int32_t getMaxAsymmetricKeyLength(SecurityPolicyId id);
      static const std::string& getKeyDerivationAlgorithm(SecurityPolicyId id);
      static const std::string& getCertificateSignatureAlgorithm(SecurityPolicyId id);

      SecurityPolicyId getPolicyId();
      std::string getPolicyUri();

      std::shared_ptr<Botan::BlockCipher> getSymmetricCryptor();
      std::shared_ptr<Botan::PK_Signer> getAsymmetricSigner();
      std::shared_ptr<Botan::PK_Verifier> getAsymmetricVerifier();
      std::shared_ptr<Botan::PK_Encryptor> getAsymmetricEncryptor();
      std::shared_ptr<Botan::PK_Decryptor> getAsymmetricDecryptor(); 
      std::shared_ptr<Botan::RandomNumberGenerator> getRandomNumberGenerator();

      X509Certificate getMyCertificate();
      X509Certificate getHisCertificate();
      uint16_t getKeysizeForSymmetricEncryption();
      uint16_t getKeysizeForSymmetricSignature();
      uint16_t getSymmPlainTextBlockSize();

      uint16_t getCreateSessionClientNonceSize();
    protected:
      static std::vector<int32_t> derivedSignatureKeyLength;
      static std::vector<int32_t> minAsymmetricKeyLength;
      static std::vector<int32_t> maxAsymmetricKeyLength;
      static std::vector<std::string> keyDerivationAlgorithm;
      static std::vector<std::string> certificateSignatureAlgorithm;

      SecurityPolicyId id;
      
      std::shared_ptr<Botan::RandomNumberGenerator> randomNumberGenerator;

      std::shared_ptr<Botan::PK_Signer> asymmetricSigner;
      std::shared_ptr<Botan::PK_Verifier> asymmetricVerifier;

      std::shared_ptr<Botan::PK_Encryptor> asymmetricEncryptor; //interface implemented in PK_Encryptor_EME
      std::shared_ptr<Botan::PK_Decryptor> asymmetricDecryptor; //interface implemented in PK_Decryptor_EME
      
      // Used for both encryption and decryption (as uses the same key for both)
      // Note that the key  needs to be set by set_key() method before starting using it.
      std::shared_ptr<Botan::BlockCipher> symmetricCryptor;



      // Certificate and private key of "this, my side" of communication. 
      // For OPC UA Client application that is client certificate.
      X509Certificate myCerticate;
      std::shared_ptr<Botan::Private_Key> myPrivateKey;

      // Certificate of the other, remote side (in case of OPC UA Client application it is certificate of UA Server to which we are connecting to).
      X509Certificate hisCerticate;
      static std::map<std::string, SecurityPolicyId> mapUriToId;
      static std::map<std::string, SecurityPolicyId> initMapUriToId();

    };
  }
}