#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include <string>
#include <stdint.h>
#include "opcua/DateTime.h"
#include "opcua/Guid.h"
#include "opcua/ByteString.h"
#include "opcua/ExpandedNodeId.h"
#include "opcua/StatusCode.h"
#include "opcua/LocalizedText.h"
#include "opcua/ExtensionObject.h"
#include "opcua/DiagnosticInfo.h"
#include <stdexcept>

namespace OWA {
	namespace OpcUa {

		// TODO!!!
		class XmlElement : public std::string 
		{
		public:
			XmlElement() {};
			XmlElement(const std::string& other);
			XmlElement(const XmlElement& other);
			virtual ~XmlElement() {}
		};

		struct Variant {
			enum struct dataType:uint8_t {
				T_Empty = 0,
				T_Boolean = 1, // A two state logical value(true or false).
				T_SByte = 2, // An integer value between −128 and 127.
				T_Byte = 3, // An integer value between 0 and 256.
				T_Int16 = 4, // An integer value between −32 768 and 32 767.
				T_UInt16 = 5, // An integer value between 0 and 65 535.
				T_Int32 = 6, // An integer value between −2 147 483 648 and 2 147 483 647.
				T_UInt32 = 7, // An integer value between 0 and 429 4967 295.
				T_Int64 = 8, // An integer value between −9 223 372 036 854 775 808 and 9 223 372 036 854 775 807
				T_UInt64 = 9, // An integer value between 0 and 18 446 744 073 709 551 615.
				T_Float = 10, // An IEEE single precision(32 bit) floating point value.
				T_Double = 11, // An IEEE double precision(64 bit) floating point value.
				T_String = 12, // A sequence of Unicode characters.
				T_DateTime = 13, // An instance in time.
				T_Guid = 14, // A 16 byte value that can be used as a globally unique identifier.
				T_ByteString = 15, // A sequence of octets.
				T_XmlElement = 16, // An XML element.
				T_NodeId = 17, // An identifier for a node in the address space of an OPC UA Server.
				T_ExpandedNodeId = 18, // A NodeId that allows the namespace URI to be specified instead of an index.
				T_StatusCode = 19, // A numeric identifier for a error or condition that is associated with a value or an operation.
				T_QualifiedName = 20, // A name qualified by a namespace.
				T_LocalizedText = 21, // Human readable text with an optional locale identifier.
				T_ExtensionObject = 22, // A structure that contains an application specific data type that may not be recognized by the receiver.
				T_DataValue = 23, // A data value with an associated status code and timestamps.
				T_Variant = 24, // A union of all of the types specified above.
				T_DiagnosticInfo = 25, // A structure that contains detailed error and diagnostic information associated with a StatusCode.
			};
			union VariantValue {
				VariantValue();
				~VariantValue();
				bool			Boolean;
				int8_t		SByte;
				uint8_t		Byte;
				int16_t		Int16;
				uint16_t	UInt16;
				int32_t		Int32;
				uint32_t	UInt32;
				int64_t		Int64;
				uint64_t	UInt64;
				float			Float;
				double		Double;
				std::string String;
				DateTime	dateTime;
				Guid			guid;
				ByteString byteString;
				XmlElement xml;
				NodeId		nodeId;
				ExpandedNodeId expandenNodeId;
				StatusCode	statusCode;
				QualifiedName qualifiedName;
				LocalizedText localizedText;
				ExtensionObject::Ptr extensionObject;
				// DataValue	dataValue;
				DiagnosticInfo diagnosticInfo;
				//Variant	variant;
				std::vector<bool> BooleanArray;
				std::vector<int8_t> SByteArray;
				std::vector<uint8_t> ByteArray;
				std::vector<int16_t> Int16Array;
				std::vector<uint16_t> UInt16Array;
				std::vector<int32_t> Int32Array;
				std::vector<uint32_t> UInt32Array;
				std::vector<int64_t> Int64Array;
				std::vector<uint64_t> UInt64Array;
				std::vector<float> FloatArray;
				std::vector<double> DoubleArray;
				std::vector<std::string> StringArray;
				std::vector<DateTime> dateTimeArray;
				std::vector<Guid> guidArray;
				std::vector<ByteString> byteStringArray;
				std::vector<XmlElement> xmlArray;
				std::vector<NodeId> nodeIdArray;
				std::vector<ExpandedNodeId> expandenNodeIdArray;
				std::vector<StatusCode> statusCodeArray;
				std::vector<QualifiedName> qualifiedNameArray;
				std::vector<LocalizedText> localizedTextArray;
				std::vector<ExtensionObject::Ptr> extensionObjectArray;
				std::vector<DiagnosticInfo> diagnosticInfoArray;
				std::vector<Variant> variantArray;
			};

			// Member variables
			VariantValue value;
			dataType type;
			bool isArray;
			uint32_t arrayLength;
			std::vector<uint32_t> arrayDimensions;

			//Constructors
			Variant();
			Variant(const Variant& other);

			Variant(const bool);
			Variant(const int8_t other);
			Variant(const uint8_t other);
			Variant(const int16_t other);
			Variant(const uint16_t other);
			Variant(const int32_t other);
			Variant(const uint32_t other);
			Variant(const int64_t other);
			Variant(const uint64_t other);
			Variant(const float other);
			Variant(const double other);
			Variant(const std::string& other);
			Variant(const DateTime& other);
			Variant(const Guid& other);
			Variant(const ByteString& other);
			Variant(const XmlElement& other);
			Variant(const NodeId& other);
			Variant(const ExpandedNodeId& other);
			Variant(const StatusCode& other);
			Variant(const QualifiedName& other);
			Variant(const LocalizedText& other);
			Variant(const ExtensionObject::Ptr& other);
			Variant(const DiagnosticInfo& other);

			Variant(const std::vector<bool>);
			Variant(const std::vector<int8_t> other);
			Variant(const std::vector<uint8_t> other);
			Variant(const std::vector<int16_t> other);
			Variant(const std::vector<uint16_t> other);
			Variant(const std::vector<int32_t> other);
			Variant(const std::vector<uint32_t> other);
			Variant(const std::vector<int64_t> other);
			Variant(const std::vector<uint64_t> other);
			Variant(const std::vector<float> other);
			Variant(const std::vector<double> other);
			Variant(const std::vector<std::string>& other);
			Variant(const std::vector<DateTime>& other);
			Variant(const std::vector<Guid>& other);
			Variant(const std::vector<ByteString>& other);
			Variant(const std::vector<XmlElement>& other);
			Variant(const std::vector<NodeId>& other);
			Variant(const std::vector<ExpandedNodeId>& other);
			Variant(const std::vector<StatusCode>& other);
			Variant(const std::vector<QualifiedName>& other);
			Variant(const std::vector<LocalizedText>& other);
			Variant(const std::vector<ExtensionObject::Ptr>& other);
			Variant(const std::vector<DiagnosticInfo>& other);
			Variant(const std::vector<Variant>& other);

			~Variant();

			// Assignment operators

			Variant& operator=(const bool);
			Variant& operator=(const int8_t other);
			Variant& operator=(const uint8_t other);
			Variant& operator=(const int16_t other);
			Variant& operator=(const uint16_t other);
			Variant& operator=(const int32_t other);
			Variant& operator=(const uint32_t other);
			Variant& operator=(const int64_t other);
			Variant& operator=(const uint64_t other);
			Variant& operator=(const float other);
			Variant& operator=(const double other);
			Variant& operator=(const std::string& other);
			Variant& operator=(const char* other);
			Variant& operator=(const DateTime& other);
			Variant& operator=(const Guid& other);
			Variant& operator=(const ByteString& other);
			Variant& operator=(const XmlElement& other);
			Variant& operator=(const NodeId& other);
			Variant& operator=(const ExpandedNodeId& other);
			Variant& operator=(const StatusCode& other);
			Variant& operator=(const QualifiedName& other);
			Variant& operator=(const LocalizedText& other);
			Variant& operator=(const ExtensionObject::Ptr& other);
			Variant& operator=(const DiagnosticInfo& other);
			Variant& operator=(const Variant& other);
			
			Variant& operator=(const std::vector<bool>);
			Variant& operator=(const std::vector<int8_t> other);
			Variant& operator=(const std::vector<uint8_t> other);
			Variant& operator=(const std::vector<int16_t> other);
			Variant& operator=(const std::vector<uint16_t> other);
			Variant& operator=(const std::vector<int32_t> other);
			Variant& operator=(const std::vector<uint32_t> other);
			Variant& operator=(const std::vector<int64_t> other);
			Variant& operator=(const std::vector<uint64_t> other);
			Variant& operator=(const std::vector<float> other);
			Variant& operator=(const std::vector<double> other);
			Variant& operator=(const std::vector<std::string>& other);
			Variant& operator=(const std::vector<DateTime>& other);
			Variant& operator=(const std::vector<Guid>& other);
			Variant& operator=(const std::vector<ByteString>& other);
			Variant& operator=(const std::vector<XmlElement>& other);
			Variant& operator=(const std::vector<NodeId>& other);
			Variant& operator=(const std::vector<ExpandedNodeId>& other);
			Variant& operator=(const std::vector<StatusCode>& other);
			Variant& operator=(const std::vector<QualifiedName>& other);
			Variant& operator=(const std::vector<LocalizedText>& other);
			Variant& operator=(const std::vector<ExtensionObject::Ptr>& other);
			Variant& operator=(const std::vector<DiagnosticInfo>& other);
			Variant& operator=(const std::vector<Variant>& other);

			bool operator==(const Variant& other) const;
			bool operator!=(const Variant& other) const;
			// Methods
			void clear();
			std::string toString(size_t maxSize = 256) const;
			
			static std::string toString(const dataType type);

			// converts to float is possible, otherwise throws exception:
			float toFloat() const;
			double toDouble() const;

			// Returns true, if data type is boolean or numeric, and either scalar or array with length 1.
			bool isNumeric() const;

			// Conversion operators. If type of isArray are not matching, throw exception.
			operator bool() const;
			operator int8_t() const;
			operator uint8_t() const;
			operator int16_t() const;
			operator uint16_t() const;
			operator int32_t() const;
			operator uint32_t() const;
			operator int64_t() const;
			operator uint64_t() const;
			operator float() const;
			operator double() const;
			operator std::string() const;
			operator DateTime() const;
			operator Guid() const;
			operator ByteString() const;
			operator XmlElement() const;
			operator NodeId() const;
			operator ExpandedNodeId() const;
			operator StatusCode() const;
			operator QualifiedName() const; 
			operator LocalizedText() const;
			operator ExtensionObject::Ptr() const;
			operator DiagnosticInfo() const;
			
			/* Parses string to convert it to current data type of the variant. If it is empty, then assigns as string
			If cannot convert, returns false */
			bool parse(const std::string& str);
			
			struct ConversionException : std::runtime_error
			{
				ConversionException(const std::string& textMessage);
			};

		private: 
			void init();
		};
	}
}