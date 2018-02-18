#pragma once
#include "opcua/DateTime.h"
#include "opcua/StatusCode.h"

namespace OWA {
  namespace OpcUa {
    struct DiagnosticInfo {
      DiagnosticInfo() {
        innerDiagnosticInfo = 0;
        symbolicId = -1;
        namespaceUri = -1;
        locale = -1;
        localizedText = -1;
        innerStatusCode = StatusCode::Good;
      }
      void clear() {
        if (innerDiagnosticInfo != 0) {
          delete innerDiagnosticInfo;
          innerDiagnosticInfo = 0;
        }
      }
      int32_t symbolicId;
      int32_t namespaceUri;
      int32_t locale;
      int32_t localizedText;
      std::string additionalInfo;
      StatusCode innerStatusCode;
      DiagnosticInfo* innerDiagnosticInfo;
			std::string toString() const {
				return "not implemented";
			}
    };
  }
}