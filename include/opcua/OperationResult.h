#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include "opcua/StatusCode.h"
#include <vector>
#include <stdint.h>
#include "opcua/LocalizedText.h"

namespace OWA {
  namespace OpcUa {
    // Struct used to return OPC UA status code and localized error message in context other than OPC UA request 
    // where there is no request header so the message cannot be returned using DiagnosticInfo and string table combination
    struct OperationResult {
      OperationResult();
			OperationResult(StatusCode code);
			OperationResult(StatusCode code, const LocalizedText& text);
			OperationResult(StatusCode code, const std::string& text);
			OperationResult(StatusCode code, const char* text);
			OperationResult(const OperationResult& other);
			OperationResult& operator=(const OperationResult& other);
			bool operator==(const OperationResult& other);
			bool isGood() const;

			std::string toString() const;
      StatusCode code;
      LocalizedText text;
    };
  }
}