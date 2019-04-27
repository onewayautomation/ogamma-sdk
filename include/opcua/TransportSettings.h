#pragma once
#pragma once 
#include <stdint.h>

namespace OWA {
	namespace OpcUa {
		struct TransportSettings {
			TransportSettings() 
			{
				setDefaults();
			};

			void setDefaults()
			{
				secureChannelLifetime = 600000; // 10 Minutes
				receiveBufferSize = 8192; // 16777216;
				sendBufferSize = 8192; // 16777216;
				maxReceiveMessageSize = 16777216; // no limit. Must be minimum 8196 bytes.
				maxSendMessageSize = 16777216;
				maxSendChunkCount = 0;
				maxReceiveChunkCount = 0;
			}

			void revise()
			{
				if (receiveBufferSize == 0)
					receiveBufferSize = 0xFFFFFFFF;
				if (sendBufferSize == 0)
					sendBufferSize = 0xFFFFFFFF;
				if (maxReceiveMessageSize == 0)
					maxReceiveMessageSize = 0xFFFFFFFF;
				if (maxSendMessageSize == 0)
					maxSendMessageSize = 0xFFFFFFFF;
				if (maxSendChunkCount == 0)
					maxSendChunkCount = 0xFFFFFFFF;
				if (maxReceiveChunkCount == 0)
					maxReceiveChunkCount = 0xFFFFFFFF;
			}

			uint32_t secureChannelLifetime;
			uint32_t receiveBufferSize; // Size of chunk for incoming messages
			uint32_t sendBufferSize; //Size of chink for outgoing messages
			uint32_t maxSendMessageSize; // Total message size for outgoing messages
			uint32_t maxReceiveMessageSize; // Total size of incoming messages
			uint32_t maxSendChunkCount; //Max count for sent chunks
			uint32_t maxReceiveChunkCount; // Max count for received chunks
		};
	}
}