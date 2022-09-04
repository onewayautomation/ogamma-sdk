#pragma once
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
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
				receiveBufferSize = 1024 * 1024; // 8192; // TODO - Setting to 8192 causes problem with Honeywell Experion.
				sendBufferSize = 1024 * 1024; // 8192; // 16777216;
				maxReceiveMessageSize = 16777216; // no limit. Must be minimum 8192 bytes.
				maxSendMessageSize = 16777216;
				maxSendChunkCount = 0;
				maxReceiveChunkCount = 0;

				newChannelRequestId = 1;
				newChannelStartingSequenceNumber = 1;
				startingSequenceNumberAfterMaxUsed = 1;
				
				//New spec: 4294967295. For older spec is 4294966271 ( https://reference.opcfoundation.org/v105/Core/docs/Part6/6.7.2/#6.7.2.4 ) 
				maxUseableSequenceNumber = 4294966271;
			}

			void revise()
			{
				if (receiveBufferSize == 0)
					receiveBufferSize = 128 * 1024 * 1024; // 128 Mb 0xFFFFFFFF;
				if (sendBufferSize == 0)
					sendBufferSize = 128 * 1024 * 1024; // 0xFFFFFFFF;
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
			uint32_t receiveBufferSize; // Size of chunk for incoming messages: must be greater than 8196
			uint32_t sendBufferSize; //Size of chink for outgoing messages: must be greater than 8196
			uint32_t maxSendMessageSize; // Total message size for outgoing messages
			uint32_t maxReceiveMessageSize; // Total size of incoming messages
			uint32_t maxSendChunkCount; //Max count for sent chunks
			uint32_t maxReceiveChunkCount; // Max count for received chunks

			// Initial values for the RequestId and Sequnce Number used in new secure channel:
			uint32_t  newChannelRequestId;

			uint32_t  newChannelStartingSequenceNumber;
			uint32_t  maxUseableSequenceNumber;
			uint32_t  startingSequenceNumberAfterMaxUsed;
		};
	}
}