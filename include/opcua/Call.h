#pragma once
#include "opcua/RequestHeader.h"
#include "opcua/ResponseHeader.h"
#include "opcua/Enums.h"
#include "opcua/NodeId.h"
#include <boost/any.hpp>
#include "opcua/Variant.h"

namespace OWA {
  namespace OpcUa {

		struct CallMethodRequest
		{
			CallMethodRequest() {};
			NodeId objectId;
			NodeId methoddId;
			std::vector<Variant> inputArguments;
		};

		struct CallMethodResult
		{
			CallMethodResult() { statusCode = StatusCode::BadInternalError; };

			StatusCode statusCode;
			std::vector<StatusCode> inputArgumentResults;
			std::vector<DiagnosticInfo> inputArgumentDiagnosticInfos;
			std::vector<Variant> outputArguments;
		};

    struct CallRequest {
			CallRequest();
			
			// Constructor to create a request to call just one method:
			CallRequest(const NodeId& objectId, const NodeId& methodId, const std::vector<Variant>& inputArguments);

			static RequestResponseTypeId getTypeId() {
        return RequestResponseTypeId::CallRequest;
      }
      RequestHeader header;
			boost::any		context;
			std::vector<CallMethodRequest> methodsToCall;

			typedef std::shared_ptr<CallRequest> Ptr;
    };
    struct CallResponse {
      ResponseHeader header;

			std::vector<CallMethodResult> results;
			std::vector<DiagnosticInfo> diagnosticInfos;

			std::shared_ptr<CallResponse> Ptr;
    };
  }
}