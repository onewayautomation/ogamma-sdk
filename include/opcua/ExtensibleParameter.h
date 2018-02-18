#pragma once
#include "opcua/DateTime.h"
#include "opcua/ExpandedNodeId.h"
#include "opcua/ByteString.h"
#include "opcua/ExtensionObject.h"

namespace OWA {
  namespace OpcUa {
    struct ExtensibleParameterBinary: public ExtensionObject 
    {
			ExtensibleParameterBinary();
      ~ExtensibleParameterBinary();
			ExtensibleParameterBinary& operator=(const ByteString& );

			ByteString body;
      
    };
		struct ExtensibleParameterXml : public ExtensionObject
		{
			ExtensibleParameterXml();
			~ExtensibleParameterXml();
			ExtensibleParameterXml& operator=(const std::string&);

			std::string body;
		};
  }
}