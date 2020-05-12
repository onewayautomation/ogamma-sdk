#pragma once
#include <vector>
#include <stdint.h>
#include <botan/secmem.h>
#include <string>

namespace OWA {
  namespace OpcUa {
    class ByteString : public std::vector<uint8_t> {

    public:
      ByteString() :std::vector<uint8_t>() {}
      ByteString(uint32_t length, uint8_t value) :std::vector<uint8_t>(length, value) {}
			ByteString(const char* value);

			ByteString& assign(uint8_t* bytes, uint32_t length);
			ByteString& append(uint8_t* bytes, uint32_t length);
			ByteString& assignCertificateFingerPrint(const std::string& other);
			ByteString& operator=(const ByteString& other);
			ByteString& operator=(const std::vector<uint8_t>& other);
			ByteString& operator=(const std::string& other);
			ByteString& operator=(const char* other);
			ByteString& operator=(const Botan::secure_vector<uint8_t>& other);
			bool operator==(const ByteString& other);

			std::string toString(size_t maxElements = 10) const;
			
			virtual ~ByteString() { }
    };

    // TODO - later implement it using Botan::secure_vector
    typedef ByteString SecureByteString;
  }

}