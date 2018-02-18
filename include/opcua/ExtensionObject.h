#pragma once
#include "opcua/ExpandedNodeId.h"

namespace OWA { namespace OpcUa {
  struct ExtensionObject {
  public:
		ExtensionObject() {}
		ExtensionObject(const ExpandedNodeId& typeId) {
			this->typeId = typeId;
		}
    ExpandedNodeId typeId;
    virtual ~ExtensionObject(){};
		std::string toString() const { return typeId.toString() + " type ExtensionObject"; } //TODO 
		typedef std::shared_ptr<ExtensionObject> Ptr;
  };

} }