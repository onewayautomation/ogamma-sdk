#pragma once
#include "opcua/RequestHeader.h"
#include "opcua/ResponseHeader.h"
#include "opcua/ByteString.h"

namespace OWA {
  namespace OpcUa {
    struct CloseSecureChannelRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::CloseSecureChannelRequest;
      }
      RequestHeader header;
      ByteString secureChannelId;

			typedef std::shared_ptr<CloseSecureChannelRequest> Ptr;
    };
    class CloseSecureChannelResponse {
    public:
      ResponseHeader header;
    };
  }
}