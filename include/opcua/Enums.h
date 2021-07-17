#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include <stdint.h>
#include <assert.h>
#include <string>
#include <sstream>

namespace OWA {
  namespace OpcUa {
    enum class SecurityTokenRequestType : uint32_t
    {
      Issue = 0,
      Renew = 1
    };

    enum class MessageSecurityMode : uint32_t
    {
      Invalid = 0,
      None = 1,
      Sign = 2,
      SignAndEncrypt = 3
    };

    // Type of the identifier in the NodeId:
    enum class IdentifierType:uint8_t {
      Numeric = 0,
      String = 1,
      Guid = 2,
      Opaque = 3
    };

    // TODO for now ids are defined for binary encoding.
    // Consider having Ids for general type, and then converting them at encoding/decoding to that type to/from encoding specific values
    enum struct RequestResponseTypeId :uint16_t {
      Connect = 0,
      Disconnect = 1,
      ServiceFaultRequest = 2, //TODO

      ServiceFault = 397, //This is response

      FindServersRequest = 422,
      FindServersResponse = 425,
      GetEndpointsRequest = 428,
      GetEndpointsResponse = 431, 

      OpenSecureChannelRequest = 446, 
      OpenSecureChannelResponse = 449,
      CloseSecureChannelRequest = 452,
      CloseSecureChannelResponse = 455,

      CreateSessionRequest = 461,
      CreateSessionResponse = 464,
      ActivateSessionRequest = 467,
      ActivateSessionResponse = 470,
      CloseSessionRequest = 473,
      CloseSessionResponse = 476,
      CancelRequest = 478,
      CancelResponse = 482,

      AddNodesRequest = 488,
      AddNodesResponse = 491,
      AddReferencesRequest = 494,
      AddReferencesResponse = 497,
      DeleteNodesRequest = 500,
      DeleteNodesResponse = 503,
      DeleteReferencesRequest = 506,
      DeleteReferencesResponse = 509,
      BrowseRequest = 527,
      BrowseResponse = 530,
      BrowseNextRequest = 533,
      BrowseNextResponse = 536,
      TranslateNodeIdsRequest = 554,
      TranslateNodeIdsResponse = 557,

      RegisterNodesRequest = 560,
      RegisterNodesResponse = 563,
      UnregisterNodesRequest = 566,
      UnregisterNodesResponse = 569,

      QueryFirstRequest = 615,
      QueryFirstResponse = 618,
      QueryNextRequest = 621,
      QueryNextResponse = 624,

      ReadRequest = 631,
      ReadResponse = 634,
      HistoryReadRequest = 664,
      HistoryReadResponse = 667,
      WriteRequest = 673,
      WriteResponse = 676,
      HistoryUpdateRequest = 700,
      HistoryUpdateResponse = 703,

      CallRequest = 712,
      CallResponse = 715, 

      CreateMonitoredItemsRequest = 751,
      CreateMonitoredItemsResponse = 754,
      ModifyMonitoredItemsRequest = 763,
      ModifyMonitoredItemsResponse = 766,
      SetMonitoringModeRequest = 769,
      SetMonitoringModeResponse = 772,
      SetTriggeringRequest = 775,
      SetTriggeringResponse = 778,
      DeleteMonitoredItemsRequest = 781,
      DeleteMonitoredItemsResponse = 784,

      CreateSubscriptionRequest = 787,
      CreateSubscriptionResponse = 790,
      ModifySubscriptionRequest = 793,
      ModifySubscriptionResponse = 796,
      SetPublishingModeRequest = 799,
      SetPublishingModeResponse = 802,
      PublishRequest = 826,
      PublishResponse = 829,
      RepublishRequest = 832,
      RepublishResponse = 835,
      TransferSubscriptionsRequest = 841,
      TransferSubscriptionsResponse = 844,
      DeleteSubscriptionsRequest = 847,
      DeleteSubscriptionsResponse = 850
    };

    enum struct NodeIdDataEncoding:uint8_t {
      TwoByte = 0x00, // A numeric value that fits into the two byte representation.
      FourByte = 0x01, // A numeric value that fits into the four byte representation.
      Numeric = 0x02, // A numeric value that does not fit into the two or four byte representations.
      String = 0x03, // A String value.
      Guid = 0x04, // A  Guid value.
      ByteString = 0x05,  // An opaque(ByteString) value.

      NamespaceUriFlag = 0x80,
      ServerIndexFlag = 0x40
    };
    // Used to compose encoding byte as combination of type of node id and flags.
    inline uint8_t operator| (NodeIdDataEncoding value1, NodeIdDataEncoding value2) { return static_cast<uint8_t>(value1) | static_cast<uint8_t>(value2); };
    inline uint8_t operator| (uint8_t value1, NodeIdDataEncoding value2) { return value1 | static_cast<uint8_t>(value2); };
    
    // Used to detect if a flag is set:
    inline uint8_t operator& (uint8_t value1, NodeIdDataEncoding value2) { return value1 & static_cast<uint8_t>(value2); };

    enum struct ExtensibleParameterBodyType:uint8_t {
      None = 0,
      ByteString = 1,
      Xml = 2,
			Json = 3
    };

    union DiagnosticInfoEncodingMask {
      typedef uint8_t integerValueType;
      integerValueType whole;

      // Could use std::bitset, but old fashion way seems simpler, better to understand
      struct bitMask
      {
        integerValueType
          SymbolicId : 1,   // 0x01	Symbolic Id
          Namespace : 1,    // 0x02	Namespace
          LocalizedText:1,  // 0x04	LocalizedText
          Locale : 1,       // 0x08	Locale
          AdditionalInfo : 1,  // 0x10	Additional Info
          InnerStatusCode : 1, //  0x20	InnerStatusCode
          InnerDiagnosticInfo : 1 //  0x40	InnerDiagnosticInfo
          ;
      } bits;
      DiagnosticInfoEncodingMask(integerValueType value = 0) : whole(value) {
        assert(sizeof(integerValueType) >= sizeof(bitMask));
      }
    };

    enum struct ApplicationType: uint32_t {
      SERVER_0 = 0, // The application is a Server.
      CLIENT_1 = 1, // The application is a Client.
      CLIENTANDSERVER_2 = 2, // The application is a Client and a Server.
      DISCOVERYSERVER_3 = 3  // The application is a DiscoveryServer.
    };

    enum struct UserIdentityTokenType :uint32_t {
      ANONYMOUS_0 = 0,  //No token is required.
      USERNAME_1 = 1,   // A username / password token.
      CERTIFICATE_2 = 2,// An X509v3 certificate token.
      ISSUEDTOKEN_3 = 3 // Any WS - Security defined token.
    };

    enum struct ConnectionState : uint8_t {
      Invalid = 0,
      Disconnected = 1, // Disconnected (initial state, or state as result of normal disconnect call)
      Connecting = 2,       // Connection is in progress
      Connected = 3,        // Connection is established
      Disconnecting = 4     // Disconnection is in progress
    };
    enum struct SecurityPolicyId : uint8_t {
      Invalid = 0,
      None = 1,
      Basic128Rsa15 = 2,
      Basic256 = 3,
      Basic256Sha256 = 4
    };

    struct SecurityMode {
      SecurityMode() {
        policyId = SecurityPolicyId::Basic256Sha256;
        messageSecurityMode = MessageSecurityMode::SignAndEncrypt;
      }
      SecurityMode(SecurityPolicyId id, MessageSecurityMode mode) {
        policyId = id;
        messageSecurityMode = mode;
      }
      static SecurityMode noneSecureMode() {
        return SecurityMode(SecurityPolicyId::None, MessageSecurityMode::None);
      }
      bool operator!=(const SecurityMode& other) {
        return (policyId != other.policyId || messageSecurityMode != other.messageSecurityMode);
      }

      bool requiresCertificate() {
        return (messageSecurityMode == MessageSecurityMode::Sign) || (messageSecurityMode == MessageSecurityMode::SignAndEncrypt);
      }

      SecurityPolicyId policyId;
      MessageSecurityMode messageSecurityMode;
    };

    namespace Protocol {
      const std::string tcp = "opc.tcp";
      const std::string https = "opc.https";
      const std::string amqp = "opc.amqp";
    };

    typedef double Duration;
		enum struct TimestampsToReturn: uint32_t {
			Source = 0,		// Return the source timestamp.
			Server = 1,		// Return the Server timestamp.
			Both = 2,			// Return both the source and Server timestamps.
			Neither = 3,	// Return neither timestamp.  This is the default value for MonitoredItems if a Variable value is not being accessed.
											// For HistoryRead this is not a valid setting.
		};

		struct QualifiedName {
			QualifiedName() {
				namespaceIndex = 0;
			}
      QualifiedName(const std::string& name_, uint16_t index_ = 0): name(name_), namespaceIndex(index_)
      {
      }
      QualifiedName(const std::string& name_) : name(name_), namespaceIndex(0)
      {

      }
      QualifiedName(const char* name_) : name(name_ == nullptr ? "" : name_), namespaceIndex(0)
      {

      }

			bool operator==(const QualifiedName& other) const
			{
				if (this == &other)
				{
					return true;
				}
				if (namespaceIndex == other.namespaceIndex && name == other.name)
				{
					return true;
				}
				return false;
			}
      bool operator!=(const QualifiedName& other) const
      {
        return !(*this == other);
      }
      
      bool operator==(const char* other) const
      {
        if (namespaceIndex == 0 && name == other)
        {
          return true;
        }
        return false;
      }
      bool operator==(const std::string& other) const
      {
        if (namespaceIndex == 0 && name == other)
        {
          return true;
        }
        return false;
      }

      bool operator<(const QualifiedName& other) const
      {
        if (this == &other)
          return false;
        if (namespaceIndex < other.namespaceIndex)
          return true;
        else if (namespaceIndex > other.namespaceIndex)
          return false;
        if (name < other.name)
          return true;
        return false;
      }
      bool operator<=(const QualifiedName& other) const
      {
        if (this == &other)
          return true;
        
        if (namespaceIndex < other.namespaceIndex)
          return true;
        else if (namespaceIndex > other.namespaceIndex)
          return false;

        if (name <= other.name)
          return true;
        return false;
      }
      bool operator>(const QualifiedName& other) const
      {
        if (this == &other)
          return false;
        if (namespaceIndex > other.namespaceIndex)
          return true;
        else if (namespaceIndex < other.namespaceIndex)
          return false;
        if (name > other.name)
          return true;
        return false;
      }
      bool operator>=(const QualifiedName& other) const
      {
        if (this == &other)
          return true;
        if (namespaceIndex > other.namespaceIndex)
          return true;
        else if (namespaceIndex < other.namespaceIndex)
          return false;

        if (name >= other.name)
          return true;
        return false;
      }

			std::string toString() const { 
				std::stringstream s;
				if (namespaceIndex > 0)
				{
					s << "ns=" << namespaceIndex << ":";
				}
				s << name;

				return s.str();
			}

      bool parse(const std::string& value);

      QualifiedName& operator=(const std::string& name_) {
        this->name = name_;
        return *this;
      }
      QualifiedName& operator=(const char* name_) {
        this->name = name_ != nullptr ? name_ : "";
        return *this;
      }

      bool isEmpty() {
        return this->namespaceIndex == 0 && this->name.empty();
      }
			uint16_t namespaceIndex;

		// protected:
			std::string name; // Should be restricted to 512 characters
		};

		enum class BrowseDirection: uint8_t {
			forward = 0,
			inverse = 1,
			both = 2
		};

		struct NodeClassBits {
			uint8_t Object : 1;
			uint8_t Variable : 1;
			uint8_t Method : 1;
			uint8_t ObjectType : 1;
			uint8_t VariableType : 1;
			uint8_t ReferenceType : 1;
			uint8_t DataType : 1;
			uint8_t View : 1;
		};

		union NodeClassMask {
			NodeClassMask() { value = 0xFF; }
			uint32_t value;
			NodeClassBits bits;
		};

		struct BrowseResultBits {
			uint8_t ReferenceType : 1;
			uint8_t IsForward : 1;
			uint8_t NodeClass : 1;
			uint8_t BrowseName : 1;
			uint8_t DisplayName : 1;
			uint8_t TypeDefinition : 1;
		};
		union BrowseResultMask {
			BrowseResultMask() {
				value = 0x3F;
			}
			uint32_t value;
			BrowseResultBits bits;
		};
		enum struct MonitoringMode: uint8_t {
			disabled = 0,
			sampling = 1,
			reporting = 2
		};

		enum struct AttributeId: uint8_t
		{
			nodeId			= 1,
			nodeClass		= 2,
			browseName	= 3,
			displayName = 4,
			description = 5,
			writeMask		= 6,
			userWriteMask = 7,
			isAbstract		= 8,
			symmetric			= 9,
			inverseName		= 10,
			containsNoLoops	= 11,
			eventNotifier		= 12,
			value						= 13,
			dataType				= 14,
			valueRank				= 15,
			arrayDimensions	= 16,
			accessLevel			= 17,
			userAccessLevel	= 18,
			minimumSamplingInterval	= 19,
			historizing			= 20,
			executable			= 21,
			userExecutable	= 22,
      dataTypeDefinition = 23,
      rolePermissions = 24,
      userRolePermissions = 25,
      accessRestrictions = 26,
      accessLevelEx = 27
		};

		// NodeClass enum values are defined in Part 3.
		enum class NodeClass: uint8_t
		{
			unspecifided = 0,
			object = 1,
			variable = 2,
			method = 4,
			objectType = 8,
			variableType = 16,
			referenceType = 32,
			dataType = 64,
			view = 128
		};

		typedef uint32_t Counter;

		enum class PerformUpdateType:uint8_t
		{
			insert = 1,
			replace = 2,
			update = 3
		};

  }
}