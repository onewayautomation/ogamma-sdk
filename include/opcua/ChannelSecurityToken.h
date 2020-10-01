#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include "opcua/DateTime.h"

namespace OWA {
  namespace OpcUa {
    class ChannelSecurityToken {
    public:
      ChannelSecurityToken() {
        SecureChannelId = 0;
        TokenId = 0;
        RevisedLifetime = 5000;
				CreatedAt.now();
      }
      uint32_t SecureChannelId;
      uint32_t TokenId;
      DateTime CreatedAt;
      int32_t RevisedLifetime;
    };
  }
}