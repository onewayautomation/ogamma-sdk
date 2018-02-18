#pragma once
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
      OperationResult(StatusCode code) {
        this->code = code;
      }
      OperationResult(StatusCode code, const LocalizedText& text) {
        this->code = code;
        this->text = text;
      }
	  	OperationResult(StatusCode code, const std::string& text) {
		  	this->code = code;
		  	this->text = text;
	  	}
			OperationResult(StatusCode code, const char* text) {
				this->code = code;
				this->text = text;
			}
      OperationResult(const OperationResult& other) {
        *this = other;
      }
      OperationResult& operator=(const OperationResult& other) {
        if (this != &other)
        {
          this->code = other.code;
          this->text = other.text;
        }
        return *this;
      }
      bool operator==(const OperationResult& other) {
        return this->code == other.code && this->text == other.text;
      }
      bool isGood() const {
        if ( (((uint32_t) code) & 0x80000000) == 0) {
          return true;
        }
        return false;
      }
      StatusCode code;
      LocalizedText text;
    };
  }
}