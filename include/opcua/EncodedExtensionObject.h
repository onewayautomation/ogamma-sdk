#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include "opcua/ExtensionObject.h"
#include "opcua/DataBuffer.h"

namespace OWA {
	namespace OpcUa {
		// Generic Structure, which can encapsulate value of any custom type structure, opaque for the SDK.
		// Assumption is that server or client applications are aware how to encode or decode them.
		// When the client SDK receives value of ExtensionObject subtype, which is not known for it, then it decoded it as EncodedExtentionObject.
		// The higher level application then can figure out, what is its data type, based on typeId, and decode if required.
		// If complex data type values are required to be stored in the database, then body of extension object can be stored as is, without decoding.
		struct EncodedExtensionObject : public ExtensionObject
		{
			EncodedExtensionObject();
			EncodedExtensionObject(const ExpandedNodeId& typeId, ExtensibleParameterBodyType bodyType, const ByteString& body);
			EncodedExtensionObject(const ExpandedNodeId& typeId, ExtensibleParameterBodyType bodyType, DataBuffer& dataBuffer);
			ExtensibleParameterBodyType bodyType;
			ByteString body;
		};

	}
}
