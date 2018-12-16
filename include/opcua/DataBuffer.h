#pragma once
#include <vector>
#include <stdint.h>
#include <memory>

namespace OWA {
  namespace OpcUa {
    class DataBuffer {
    public:
      DataBuffer() {
        position = 0;
      }

      DataBuffer(uint32_t count, uint8_t value = 0) : buffer(count, value) {
        position = 0;
      }
      DataBuffer& operator=(const std::vector<uint8_t>& other) {
        this->buffer = other;
        position = 0;
        return *this;
      }

      uint8_t& operator[](uint32_t index) {
        return buffer[index];
      }
      uint8_t* data() {
        return buffer.data();
      }
      uint32_t size() {
        return (uint32_t) buffer.size();
      }
      void sgetn(uint8_t* m, uint32_t count) {
				if ((position + count) > buffer.size())
				{
					std::stringstream stream;
					stream << "Decoding error. Buffer with length " << buffer.size() << " is being accessed from position " << position << " to " << (position + count);
					throw OperationResult(StatusCode::BadDecodingError, stream.str());
				}
        memcpy(m, buffer.data() + position, count);
        position += count;
      }
      void sgetn(void* m, uint32_t count) {
        sgetn((uint8_t*)m, count);
      }

      void sputn(const uint8_t* m, uint32_t count) {
        for (uint32_t index = 0; index < count; index++) {
          buffer.push_back( *(m + index) );
        }
      }
      void sputn(const void* m, uint32_t count) {
        sputn((const uint8_t*)m, count);
      }

      void replace(uint32_t pos, const uint8_t* m, uint32_t count) {
        memcpy(buffer.data() + pos, m, count);
      }
      void replace(uint32_t pos, const void* m, uint32_t count) {
        replace(pos, (const uint8_t*)m, count);
      }


      void reserve(uint32_t count) {
        buffer.reserve(count);
      }
      void setPosition(uint32_t position) {
        this->position = position;
      }
      uint32_t getPosition() {
        return position;
      }

      void resize(uint32_t newSize, const uint8_t& value = 0) {
        buffer.resize(newSize, value);
      }
      void swap(std::vector<uint8_t>& other) {
        buffer.swap(other);
      }

    protected:
      std::vector<uint8_t> buffer;
      uint32_t position;
    };

    typedef std::shared_ptr<DataBuffer> DataBufferPtr;

  }
}