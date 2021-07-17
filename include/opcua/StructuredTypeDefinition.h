#pragma once
#include "opcua/Enums.h"
#include "opcua/ExpandedNodeId.h"
#include "opcua/OperationResult.h"
#include <string>
#include <vector>
#include <map>
#include <mutex>

namespace OWA {
  namespace OpcUa {
    struct ExpandedComplexValue;

    enum struct StructureType: uint32_t {
      Structure = 0,
      StructureWithOptionalFields = 1,
      Union = 2
    };

    struct StructureField {
      std::string name;
      LocalizedText description;
      NodeId dataType;
      int32_t valueRank;
      std::vector<uint32_t> arrayDimensions;
      uint32_t maxStringLength;
      bool isOptional;
    };

    // To represent value of the DataTypeDefinition attribute of the data type node which is sub-type of the Structure type.
    struct StructureDefinition
    {
      NodeId defaultEncodingId;
      NodeId baseDataType;
      StructureType type; 
      std::vector<StructureField> fields;
    };

    struct StructuredTypeDefinition;

    // Namespace Manager is used to maintain namespace URIs and compose Qualified Names from names defined with aliases, like "opc:Boolean".
    struct NamespaceManager
    {
      NamespaceManager();

      std::shared_ptr<StructuredTypeDefinition> getTypeDefinition(const ExpandedNodeId& typeNodeId);
      std::shared_ptr<StructuredTypeDefinition> getTypeDefinition(const QualifiedName& typeName);
      bool addTypeDefinition(const QualifiedName& typeName, const NodeId& nodeId, 
        const ExpandedNodeId& typeNodeId, std::shared_ptr<StructuredTypeDefinition> td, bool allowDuplicate = false);

      void addSessionNamespace(const std::string& uri, const std::string& alias = "");
      void setSessionNamespaces(const std::vector<std::string>& uris);

      // Resets aliases, intended to be used at the start of parsing type definition XML file. 
      void resetAliases();

      uint16_t getIndexByAlias(const std::string& alias);
      uint16_t getIndexByUri(const std::string& uri);

      bool hasNamespaceUri(const std::string& uri);

      std::string getAliasByUri(const std::string& uri);

      void setPersistentNamespaces(const std::vector<std::string>& namespaces_);
      
      // Creates Qualified name from name in format "[alias]:name", for example, "opc:Boolean".
      // If alias not found, then set namespace index to 0.
      QualifiedName toQualifiedName(const std::string& nameWithAlias, uint16_t defaultNamespaceIndex = 0);
      QualifiedName getTypeName(const NodeId&) const;
      QualifiedName getTypeNameByTypeDefNodeId(const NodeId&) const;

      bool canHaveFields(const QualifiedName& typeName);

      void clear();

      OperationResult serializeToString(std::string& output);
      OperationResult deserializeFromString(const std::string& input);

      std::shared_ptr<std::recursive_mutex> getMutex();

      std::string getName(); // Returns name of the parent Connection object.
      void setName(const std::string& newName);

    // protected:
      std::string name;
      std::vector<std::string> persistentNamespaces;
      std::vector<std::string> sessionNamespaces;
      std::map<std::string, std::string> mapAliasToUri;
      std::map<std::string, uint32_t> mapUriToIndex;

      // Map to find type name by Extension Object data type node id:
      std::map<NodeId, QualifiedName> mapObjectTypeIdToTypeName;
      // Map to find StructuredTypeDefinition by type name:
      std::map<QualifiedName, std::shared_ptr<StructuredTypeDefinition>> mapTypeDictionary;
      // Map to find StructuredTypeDefinition by type node id:
      std::map<NodeId, std::shared_ptr<StructuredTypeDefinition>> mapObjectTypeIdToTypeDefinition;
      
      // Map to find StructuredTypeDefinition by Node Id of the 1.04 type definition node (nodes - subtypes of the Structure node ).
      std::map<NodeId, std::shared_ptr < StructuredTypeDefinition>> mapTypeDefNodeIdToTypeDefinition;
      
      // Mutex to protect the maps:
      std::shared_ptr<std::recursive_mutex> typeDictionaryMutex;

      OperationResult parseXmlTypeDefinition(std::string& xml, bool readFromFile = false);
      OperationResult parseNamespacesFromXmlTypeDefinition(std::string& xml);

      OperationResult revise();
    };

    struct FieldType {
      FieldType();
      bool operator==(const FieldType& other) const;
      bool operator!=(const FieldType& other) const;
      QualifiedName typeName;
      NodeId typeNodeId;
      std::string name;
      std::string lengthField;
      std::string switchField;
      
      size_t lengh;

      bool isLengthInBytes;
      
      // Enumerated value:
      uint32_t value;

      // To copy valueRank member from StructureDefintion:
      int32_t valueRank;
    };

    struct StructuredTypeDefinition 
    {
      enum struct Types: uint8_t {
        OpaqueType = 0,
        EnumeratedType = 1,
        StructuredType = 2
      };

      StructuredTypeDefinition();
      StructuredTypeDefinition(const QualifiedName& name_, const QualifiedName& baseType_ = "");

      bool operator==(const StructuredTypeDefinition& other) const;
      bool operator!=(const StructuredTypeDefinition& other) const;

      QualifiedName name;
      QualifiedName baseType;
      NodeId baseTypeId;

      std::vector<FieldType> fields;

      Types type;

      uint16_t lenghInBits;
    };
  }
}
