#pragma once
#include "opcua/Cryptor.h"

namespace OWA {
  namespace OpcUa {
    class BaseCryptor : public Cryptor {
      virtual void signAndEncrypt(DataBufferPtr&  buffer) {};
    };
  }
}