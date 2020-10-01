#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include "opcua/ExpandedNodeId.h"

namespace OWA { namespace OpcUa {
  struct ExtensionObject {
		ExtensionObject() {}
		ExtensionObject(const ExpandedNodeId& typeId):typeId(typeId) {
		}
    ExpandedNodeId typeId;
		bool operator==(const ExtensionObject& other) const;
    virtual ~ExtensionObject(){};
		virtual std::string toString() const { return typeId.toString() + " type ExtensionObject"; } //TODO 
		typedef std::shared_ptr<ExtensionObject> Ptr;
  };

} }