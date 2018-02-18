#pragma once
#include "opcua/RequestHeader.h"
#include "opcua/ResponseHeader.h"
#include "opcua/Enums.h"
#include "opcua/NumericNodeId.h"
#include "opcua/DataValue.h"

namespace OWA {
  namespace OpcUa {
		typedef uint32_t IntegerId;
		typedef std::string NumericRange;
		
		struct ReadValueId {
			ReadValueId() {
				attributeId = 13;
			}
			ReadValueId(const NodeId& nodeId, IntegerId attributeId = 13) {
				this->nodeId = nodeId;
				this->attributeId = attributeId;
			}
			ReadValueId(uint32_t intNodeId, uint16_t namespaceIndex = 0) : nodeId(intNodeId, namespaceIndex) {
				attributeId = 13;
			}
			NodeId				nodeId;
			IntegerId			attributeId;
			NumericRange	indexRange;
			QualifiedName dataEncoding;
		};

    struct ReadRequest {
			ReadRequest() {
				maxAge = 0;
				timestampsToReturn = TimestampsToReturn::Both;
			}
			ReadRequest(const NodeId& nodeId):ReadRequest() {
				nodesToRead.push_back(nodeId);
			}
			virtual ~ReadRequest() {
				int i = 0;
			}

      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::ReadRequest;
      }
      RequestHeader				header;
			Duration						maxAge;
			TimestampsToReturn	timestampsToReturn;
			std::vector<ReadValueId>	nodesToRead;
			
			typedef std::shared_ptr<ReadRequest> Ptr;
    };

    struct ReadResponse {
      ResponseHeader header;
			std::vector<DataValue> results;
			std::vector<DiagnosticInfo> diagnosticInfos;
			inline bool isGood() { return Utils::isGood(*this);}
			typedef std::shared_ptr<ReadResponse> Ptr;
			~ReadResponse() {
				int i = 0;
			}
    };

    struct WriteRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::WriteRequest;
      }
      RequestHeader header;

			typedef std::shared_ptr<WriteRequest> Ptr;
    };
    struct WriteResponse {
      ResponseHeader header;
    };
    struct HistoryReadRequest{
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::HistoryReadRequest;
      }
      RequestHeader header;

			typedef std::shared_ptr <HistoryReadRequest> Ptr;
    };
    struct HistoryReadResponse {
      ResponseHeader header;

			typedef std::shared_ptr<HistoryReadResponse> Ptr;
    };

    struct HistoryUpdateRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::HistoryUpdateRequest;
      }
      RequestHeader header;

			typedef std::shared_ptr<HistoryUpdateRequest> Ptr;
    };
    struct HistoryUpdateResponse {
      ResponseHeader header;

			typedef std::shared_ptr<HistoryUpdateResponse> Ptr;
    };

  }
}