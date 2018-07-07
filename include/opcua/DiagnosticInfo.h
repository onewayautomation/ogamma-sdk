#pragma once
#include "opcua/DateTime.h"
#include "opcua/StatusCode.h"

namespace OWA {
  namespace OpcUa {
    struct DiagnosticInfo {
			DiagnosticInfo();
			DiagnosticInfo(const DiagnosticInfo& other);
			~DiagnosticInfo();
			void clear();

			bool operator==(const DiagnosticInfo& other) const;

			DiagnosticInfo& operator=(const DiagnosticInfo& other);

      int32_t symbolicId;
      int32_t namespaceUri;
      int32_t locale;
      int32_t localizedText;
      std::string additionalInfo;
      StatusCode innerStatusCode;
      DiagnosticInfo* innerDiagnosticInfo;
			std::string toString() const;
    };
	}
}