#pragma once
#include <botan/botan.h>
#include <vector>
#include <stdint.h>

namespace OWA {
  namespace OpcUa {
    class ByteString : public std::vector<uint8_t> {

    public:
      ByteString() :std::vector<uint8_t>() {}
      ByteString(uint32_t length, uint8_t value) :std::vector<uint8_t>(length, value) {}

      ByteString& operator=(const std::vector<uint8_t>& other) {
        clear();
        if (other.size() > 0) {
          reserve(other.size());
          for (auto iter = other.begin(); iter != other.end(); iter++) {
            push_back((uint8_t)*iter);
          }
        }
        return *this;
      };

      ByteString& operator=(const std::string& other) {
        clear();
        if (other.size() > 0) {
          reserve(other.size());
          for (auto iter = other.begin(); iter != other.end(); iter++) {
            push_back((uint8_t) *iter);
          }
        }
        return *this;
      }
      ByteString& assignCertificateFingerPrint(const std::string& other) {
        clear();
        if (!other.empty()) {
          reserve(20);
          for (size_t index = 0; index < other.size(); index+=3) {
            uint8_t i = std::strtoul(&other[index], 0, 16);
            push_back(i);
          }
        }
        return *this;
      }
      ByteString& operator=(const Botan::secure_vector<uint8_t>& other) {
        this->clear();
        reserve(other.size());
        for (auto iter = other.begin(); iter != other.end(); iter++) {
          this->push_back(*iter);
        }
        return *this;
      }
      ByteString& assign(uint8_t* bytes, uint32_t length) {
        if (size() != length) {
          clear();
          reserve(length);
          for (uint32_t index = 0; index < length; index++) {
            push_back(*(bytes + index));
          }
        }
        else {
          memcpy(data(), bytes, length);
        }
        return *this;
      }

			std::string toString() const;

    };

    // TODO - later implement it using Botan::secure_vector
    typedef ByteString SecureByteString;
  }

}