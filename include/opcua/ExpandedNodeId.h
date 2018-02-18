#pragma once
#include "opcua/NodeId.h"
#include "opcua/NumericNodeId.h"

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
      bool operator==(const ExpandedNodeId other);
      uint32_t    serverIndex;
      std::string namespaceURI;
      NodeId      nodeId;
			std::string toString() const;
    protected:
      static std::map<RequestResponseTypeId, ExpandedNodeId> dataTypeIds;
    };
  }
}