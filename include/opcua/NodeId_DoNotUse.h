#include <stdint.h>
#include <string>
#include <vector>

namespace OWA {
  namespace OpcUa {

    enum NodeIdEncoding : uint8_t
    {
      EV_TWO_BYTE = 0,
      EV_FOUR_BYTE = 1,
      EV_NUMERIC = 2,
      EV_STRING = 3,
      EV_GUId = 4,
      EV_BYTE_STRING = 5,

      EV_Server_INDEX_FLAG = 0x40,
      EV_NAMESPACE_URI_FLAG = 0x80,

      EV_VALUE_MASK = 0x07 // ignore bits other than 1, 2 and 3 - they can be set to 1. 
    };

    struct ExpandedNodeId;

    struct NodeId
    {
      NodeIdEncoding encoding;
      std::string namespaceURI;
      uint32_t serverIndex;

      struct TwoByteDataType
      {
        uint8_t identifier;

        TwoByteDataType()
          : identifier(0)
        {
        }

      } twoByteData;

      struct FourByteDataType
      {
        uint8_t namespaceIndex;
        uint16_t identifier;

        FourByteDataType()
          : namespaceIndex(0)
          , identifier(0)
        {
        }
      } fourByteData;

      struct NumericDataType
      {
        uint16_t namespaceIndex;
        uint32_t identifier;

        NumericDataType()
          : namespaceIndex(0)
          , identifier(0)
        {
        }
      } numericData;


      struct StringDataType
      {
        uint16_t namespaceIndex;
        std::string identifier;

        StringDataType()
          : namespaceIndex(0)
        {
        }

      } stringData;

      struct BinaryDataType
      {
        uint16_t namespaceIndex;
        std::vector<uint8_t> identifier;

        BinaryDataType()
          : namespaceIndex(0)
        {
        }

      } binaryData;

      struct GuidDataType
      {
        uint16_t namespaceIndex;
        Guid identifier;

        GuidDataType()
          : namespaceIndex(0)
        {
        }

      } guidData;

      NodeId();
      NodeId(const NodeId& node);
      NodeId(const ExpandedNodeId& node);
      NodeId(MessageId messageId);
      NodeId(ReferenceId referenceId);
      NodeId(ObjectId objectId);
      NodeId(ExpandedObjectId objectId);
      NodeId(uint32_t integerId, uint16_t index);
      NodeId(std::string stringId, uint16_t index);

      NodeId& operator= (const NodeId& node);
      NodeId& operator= (const ExpandedNodeId& node);

      explicit operator ExpandedNodeId();

      NodeId& operator= (MessageId messageId)
      {
        *this = NodeId(messageId);
        return *this;
      }

      NodeId& operator= (ReferenceId referenceId)
      {
        *this = NodeId(referenceId);
        return *this;
      }

      NodeId& operator= (ObjectId objectId)
      {
        *this = NodeId(objectId);
        return *this;
      }

      NodeId& operator= (ExpandedObjectId objectId)
      {
        *this = NodeId(objectId);
        return *this;
      }

      bool operator== (const NodeId& node) const;
      bool operator== (MessageId messageId) const;
      bool operator== (ReferenceId referenceId) const;
      bool operator== (ObjectId objectId) const;
      bool operator== (ExpandedObjectId objectId) const;

      bool operator!= (const NodeId& node) const;
      bool operator!= (MessageId messageId) const;
      bool operator!= (ReferenceId referenceId) const;
      bool operator!= (ObjectId objectId) const;
      bool operator!= (ExpandedObjectId objectId) const;

      bool operator< (const NodeId& node) const;

      NodeIdEncoding GetEncodingValue() const;
      bool HasNamespaceURI() const;
      bool HasServerIndex() const;

      void SetNamespaceURI(const std::string& uri);
      void SetServerIndex(uint32_t index);
      void SetNamespaceIndex(uint32_t ns);

      bool IsInteger() const;
      bool IsString() const;
      bool IsBinary() const;
      bool IsGuid() const;

      uint32_t GetNamespaceIndex() const;

      uint32_t GetIntegerIdentifier() const;
      std::string GetStringIdentifier() const;
      std::vector<uint8_t> GetBinaryIdentifier() const;
      Guid GetGuidIdentifier() const;

    protected:
      void CopyNodeId(const NodeId& node);
    };

    inline NodeId TwoByteNodeId(uint8_t value)
    {
      NodeId id;
      id.encoding = EV_TWO_BYTE;
      id.twoByteData.identifier = value;
      return id;
    }

    inline NodeId FourByteNodeId(uint16_t value, uint8_t namespaceIndex = 0)
    {
      NodeId id;
      id.encoding = EV_FOUR_BYTE;
      id.fourByteData.identifier = value;
      id.fourByteData.namespaceIndex = namespaceIndex;
      return id;
    }

    inline NodeId NumericNodeId(uint32_t value, uint16_t namespaceIndex = 0)
    {
      NodeId id;
      id.encoding = EV_NUMERIC;
      id.numericData.identifier = value;
      id.numericData.namespaceIndex = namespaceIndex;
      return id;
    }

    inline NodeId StringNodeId(std::string value, uint16_t namespaceIndex = 0)
    {
      NodeId id;
      id.encoding = EV_STRING;
      id.stringData.identifier = value;
      id.stringData.namespaceIndex = namespaceIndex;
      return id;
    }

    inline NodeId BinaryNodeId(std::vector<uint8_t> value, uint16_t namespaceIndex = 0)
    {
      NodeId id;
      id.encoding = EV_BYTE_STRING;
      id.binaryData.identifier = value;
      id.binaryData.namespaceIndex = namespaceIndex;
      return id;
    }

    inline NodeId GuidNodeId(Guid value, uint16_t namespaceIndex = 0)
    {
      NodeId id;
      id.encoding = EV_GUId;
      id.guidData.identifier = value;
      id.guidData.namespaceIndex = namespaceIndex;
      return id;
    }

    struct ExpandedNodeId : public NodeId
    {
      ExpandedNodeId();
      ExpandedNodeId(const NodeId& node);
      ExpandedNodeId(const ExpandedNodeId& node);
      ExpandedNodeId(MessageId messageId);
      ExpandedNodeId(ReferenceId referenceId);
      ExpandedNodeId(ObjectId objectId);
      ExpandedNodeId(ExpandedObjectId objectId);
      ExpandedNodeId(uint32_t integerId, uint16_t index);
      ExpandedNodeId(std::string stringId, uint16_t index);

      //using NodeId::NodeId;
      //using base::base;
    };

    struct SessionAuthenticationToken : public ExpandedNodeId {

    };
  }
}