#pragma once
#include "opcua/Configurable.h"
#include <string>

namespace OWA {
  namespace OpcUa {
    class Configurable {
      /**
      * Returns metadata which describes configuration parameters (schema) in JSON format
      */
      virtual std::string getConfigurationMetadata(const::std::string& locale) {
        return "";
      }

      /**
      * Returns current configuration in JSON format
      */
      virtual std::string getConfiguration() {
        return "";
      }

      /**
      * Sets new configuration in JSON format
      */
      virtual void setConfiguration(const std::string& configuration) {};
    };
  }
}