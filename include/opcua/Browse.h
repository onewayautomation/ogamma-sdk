#pragma once
#include "opcua/RequestHeader.h"
#include "opcua/ResponseHeader.h"
#include "opcua/Enums.h"
#include "opcua/NumericNodeId.h"
#include <boost/any.hpp>

namespace OWA {
  namespace OpcUa {
		struct ViewDescription {
			ViewDescription() { version = 0; }
			NodeId		id;
			DateTime	timestamp;
			uint32_t	version;
		};

		struct BrowseDescription {
			BrowseDescription() {
				browseDirection = BrowseDirection::forward;
				includeSubtypes = true;
				referenceTypeId = Ids::NumericNodeId::HierarchicalReferences;
			}
			BrowseDescription(const NodeId& nodeId) : BrowseDescription() {
				this->nodeId = nodeId;
			}
			NodeId nodeId;
			BrowseDirection browseDirection;
			NodeId referenceTypeId;
			bool includeSubtypes;
			NodeClassMask nodeClassMask;
			OpcUa::BrowseResultMask resultMask;
		};

		struct ReferenceDesciption {
			NodeId referenceTypeId;
			bool isForward;
			ExpandedNodeId nodeId;
			QualifiedName browseName;
			LocalizedText displayName;
			NodeClass nodeClass;
			ExpandedNodeId typeDefiniton;
		};

		struct BrowseResult {
			StatusCode statusCode;
			ByteString continuationPoint;
			std::vector<ReferenceDesciption> references;
		};
    struct BrowseRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::BrowseRequest;
      }
			BrowseRequest() {
				requestedMaxReferencesPerNode = 0;
			}
			BrowseRequest(const NodeId& nodeId):BrowseRequest() {
				nodesToBrowse.push_back(nodeId);
			}
      RequestHeader			header;
			ViewDescription		view;
			uint32_t					requestedMaxReferencesPerNode;
			std::vector<BrowseDescription> nodesToBrowse;
			boost::any				context;
			typedef std::shared_ptr<BrowseRequest> Ptr;
			~BrowseRequest() {
				int i = 0; //TODO
			}
    };

    struct BrowseResponse {
      ResponseHeader header;
			std::vector<BrowseResult> results;
			std::vector<DiagnosticInfo> diagnosticInfos;
			typedef std::shared_ptr<BrowseResponse> Ptr;
    };

    struct BrowseNextRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::BrowseNextRequest;
      }
			BrowseNextRequest() {
				releaseContinuationPoints = false;
			}
			void clear() {
				continuationPoints.clear();
				header.utcTime = 0;
			}
      RequestHeader header;
			boost::any		context;
			bool releaseContinuationPoints;
			std::vector<ByteString> continuationPoints;
			typedef std::shared_ptr<BrowseNextRequest> Ptr;
    };

    struct BrowseNextResponse {
      ResponseHeader header;
			std::vector<BrowseResult> results;
			std::vector<DiagnosticInfo> diagnosticInfos;

			typedef std::shared_ptr<BrowseNextResponse> Ptr;
    };

    struct TranslateBrowsePathsToNodeIdsRequest {
      static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::TransferSubscriptionsRequest;
      }
      RequestHeader header;
			boost::any		context;

			typedef std::shared_ptr<TranslateBrowsePathsToNodeIdsRequest> Ptr;
    };
    struct TranslateBrowsePathsToNodeIdsResponse {
      ResponseHeader header;
    };
  }
}