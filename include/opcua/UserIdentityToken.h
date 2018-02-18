#pragma once
#include "opcua/ExtensionObject.h"

namespace OWA { namespace OpcUa {
  class UserIdentityToken: public ExtensionObject {
  };

  class AnonymousIdentityToken: public UserIdentityToken {
  public:
    AnonymousIdentityToken();

    std::string policyId;
  };

} }