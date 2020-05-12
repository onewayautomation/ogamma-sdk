#pragma once
#include "opcua/ExtensionObject.h"

namespace OWA { namespace OpcUa {
  class UserIdentityToken: public ExtensionObject 
	{
	public:
		UserIdentityToken() :type(UserIdentityTokenType::ANONYMOUS_0) {};
		UserIdentityTokenType type;
		std::string policyId;
		std::string securityPolicyUri;
  };

  class AnonymousIdentityToken: public UserIdentityToken {
  public:
    AnonymousIdentityToken();
  };

	class UserNameIdentityToken : public UserIdentityToken {
	public:
		UserNameIdentityToken();
		std::string userName;
		ByteString	password;
		std::string encryptionAlgorithm;
	};

	class X509IdentityToken : public UserIdentityToken {
	public:
		X509IdentityToken();
		ByteString	certificateData;
	};

	class IssuedIdentityToken : public UserIdentityToken {
	public:
		IssuedIdentityToken();
		ByteString	tokenData;
		std::string encryptionAlgorithm;
	};
} }