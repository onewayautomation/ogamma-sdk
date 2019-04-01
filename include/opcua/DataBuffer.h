#pragma once
#include <vector>
#include <stdint.h>
#include <memory>
#include <functional>
#include "opcua/Transport.h"
#include "opcua/ByteString.h"

namespace OWA {
  namespace OpcUa {
		class Transport;
		struct EncodedExtensionObject;
		class DataBuffer {
			friend Transport;
			friend EncodedExtensionObject;
    public:
			DataBuffer();
			DataBuffer(const ByteString& other);
			// Reserves buffer size, and keeps size of buffer as 0.
			DataBuffer(uint32_t count);
			// Initializes buffer with "count" elements each equal to "value". Size of the buffer becomes equal to count.
			DataBuffer(uint32_t count, uint8_t value);

			DataBuffer& operator=(const std::vector<uint8_t>& other);
			uint8_t& operator[](uint32_t index);
			uint8_t* data();
			uint32_t size();
			void sgetn(uint8_t* m, uint32_t count);
			void sgetn(void* m, uint32_t count);
			void sputn(const uint8_t* m, uint32_t count);
			void sputn(const void* m, uint32_t count);
			void replace(uint32_t pos, const uint8_t* m, uint32_t count);
			void replace(uint32_t pos, const void* m, uint32_t count);
			void reserve(uint32_t count);
			void setPosition(uint32_t position);
			uint32_t getPosition();
			void resize(uint32_t newSize, const uint8_t& value = 0);
			void swap(std::vector<uint8_t>& other);
			void setFlag(char flag);
			char getFlag();
			void clear();
			bool incrementChunkNumber();
			uint32_t getSecurityRequestId();
			void setTransport(std::shared_ptr<Transport>& transport);
			void setSecurityRequestId(uint32_t id);
			void push_back(std::shared_ptr<DataBuffer>& other);

    protected:
			void init();
      std::vector<uint8_t> buffer; 
			uint32_t position;
			std::shared_ptr<Transport> transport;
			uint32_t totalMessageSize;
			uint32_t chunkNumber;
			char messageFlag;
			uint32_t securityRequestId;
    };

    typedef std::shared_ptr<DataBuffer> DataBufferPtr;

  }
}