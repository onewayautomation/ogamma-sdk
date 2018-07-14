#pragma once
#include "opcua/ExtensionObject.h"

namespace OWA { namespace OpcUa {
  class UserIdentityToken: public ExtensionObject 
	{
	public:
		UserIdentityTokenType type;
		std::string policyId;
  };

  class AnonymousIdentityToken: public UserIdentityToken {
  public:
    AnonymousIdentityToken();
  };

} }