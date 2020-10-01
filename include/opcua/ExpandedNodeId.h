#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include "opcua/NodeId.h"
#include "opcua/NumericNodeId.h"
#include <map>

namespace OWA {
  namespace OpcUa {
    class ExpandedNodeId {
    public:
      ExpandedNodeId();
			ExpandedNodeId(const ExpandedNodeId& other);
      ExpandedNodeId(RequestResponseTypeId id);
      ExpandedNodeId(Ids::NumericNodeId id);
      static ExpandedNodeId getId(RequestResponseTypeId id);
      RequestResponseTypeId getTypeId();
      bool operator==(const ExpandedNodeId other) const;
			bool operator==(const Ids::NumericNodeId id) const;
			bool operator==(const uint32_t id) const;
      uint32_t    serverIndex;
      std::string namespaceURI;
      NodeId      nodeId;
			std::string toString() const;
    protected:
      static std::map<RequestResponseTypeId, ExpandedNodeId> dataTypeIds;
    };
  }
}