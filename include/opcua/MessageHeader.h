#pragma once
#include "opcua/ByteString.h"
#include <stdint.h>
#include "opcua/TransportSettings.h"
#include "opcua/ExpandedNodeId.h"
#include "opcua/DataBuffer.h"

namespace OWA{
  namespace OpcUa {
      enum struct TcpMessageType:uint8_t {
        HEL = 0, 
        ACK,
        ERR, 
        OPN, 
        MSG, 
        CLO
      };
      
      enum struct TcpMessageChunkType:uint8_t {
        FinalAborted = 'A',
        Intermediate = 'C',
        Final = 'F'
      };

      struct MessageHeader {
      private:
        static const char* TcpMessageTypes[6];
      public:
        MessageHeader(TcpMessageType messageType = TcpMessageType::HEL) {
          setMessageType(messageType);
          ChunkType = static_cast<uint8_t>(TcpMessageChunkType::Final);
          MessageSize = 8;
        }
        void setMessageType(TcpMessageType messageType) {
          uint8_t type = static_cast<uint8_t>(messageType);
          assert(0 <= type && type <= (uint8_t)(sizeof(TcpMessageTypes) / sizeof(const char *)));
          memcpy(MessageType, TcpMessageTypes[type], 3);
        }

        TcpMessageType getMessageType() {
          for (int index = 0; index < 6; index++) {
            if (memcmp(MessageType, TcpMessageTypes[index], 3) == 0) {
              return static_cast<TcpMessageType>(index);
            }
          }
          return TcpMessageType::ERR; //TODO
        }

        /** Can be one of:
          * HEL a Hello Message.
          * ACK an Acknowledge Message.
          * ERR an Error Message.
          * The SecureChannel layer defines additional values which the OPC UA TCP layer shall accept.
          */
        uint8_t MessageType[3];
        uint8_t ChunkType;
        uint32_t MessageSize; // Includes length of this message (8 bytes)
      };
			typedef std::shared_ptr<MessageHeader> MessageHeaderPtr;

      struct SequenceHeader
      {
        uint32_t SequenceNumber;
        uint32_t RequestId;

        SequenceHeader() {
          SequenceNumber = 0;
          RequestId = 0;
        }
        SequenceHeader(uint32_t sequenceNumber, uint32_t requestId) {
          SequenceNumber = sequenceNumber;
          RequestId = requestId;
        }
      };

      //struct SecureConversationMessageHeader {
      //  SecureConversationMessageHeader();
      //  MessageHeader messageHeader;
      //  uint32_t SecureChannelId; // A unique identifier for the SecureChannel assigned by the Server.
      //};

      struct AssymmetricAlgortrithmSecurityHeader {
        AssymmetricAlgortrithmSecurityHeader() {
          secureChannelId = 0;
        }
        uint32_t secureChannelId;
        std::string securityPolicyUri;
        ByteString senderCertificate;
        ByteString receiverCertificateThumbprint;
        SequenceHeader sequenceHeader;
        ExpandedNodeId typeId;
      };

      // Includes extra field (secureChannelID) which literally does not belong to here, but included for convenience.
      struct SymmetricAlgorithmSecurityHeader
      {
        SymmetricAlgorithmSecurityHeader() {
          secureChannelId = 0;
          tokenId = 0;
        }
        uint32_t secureChannelId;
        uint32_t tokenId;
        SequenceHeader sequenceHeader;
        // TODO ExpandedNodeId typeNodeId;
      };

      struct SecureConversationMessageFooter {
        uint8_t PaddingSize;
        uint8_t ExtraPaddingSize;
        std::vector<uint8_t> Padding;
        std::vector<uint8_t> Signature;
      };

      struct HelloMessage {
        HelloMessage(const TransportSettings& settings, const std::string& url) {
          EndpointUrl = url;
          ProtocolVersion = 0;
					ReceiveBufferSize = settings.receiveBufferSize;
					SendBufferSize = settings.sendBufferSize;
          MaxMessageSize = settings.maxReceiveMessageSize; // no limit. Must be minimum 8192 bytes.
					if (MaxMessageSize < 8192)
						MaxMessageSize = 8192;
          if (ReceiveBufferSize < 8192)
            ReceiveBufferSize = 8192;
          if (SendBufferSize < 8192)
            SendBufferSize = 8192;

          MaxChunkCount = settings.maxReceiveChunkCount;
        }

        uint32_t ProtocolVersion;
        uint32_t ReceiveBufferSize;
        uint32_t SendBufferSize;
        uint32_t MaxMessageSize;
        uint32_t MaxChunkCount;

        /**
        * The URL of the Endpoint which the Client wished to connect to.
        * The encoded value shall be less than 4 096 bytes.
        * Servers shall return a Bad_TcpUrlRejected error and close the connection if the length exceeds 4 096 or if it does not recognize 
        * the resource identified by the URL.
        */
        std::string EndpointUrl;

        template <class Archive>
        void serialize(Archive & ar) const
        {
          ar(ProtocolVersion, ReceiveBufferSize, SendBufferSize, MaxMessageSize, MaxChunkCount, EndpointUrl);
        }
      };

      struct AchnowledgeMessage {
        AchnowledgeMessage() {
          ProtocolVersion = 0;
          ReceiveBufferSize = 0;
          SendBufferSize = 0;
          MaxMessageSize = 0;
          MaxChunkCount = 1;
        }
        uint32_t ProtocolVersion;
        uint32_t ReceiveBufferSize;
        uint32_t SendBufferSize;
        uint32_t MaxMessageSize;
        uint32_t MaxChunkCount;
      };

      struct ErrorMessage {
        ErrorMessage() :error(0){};
        uint32_t error;
        std::string reason;
      };

			struct TcpReadContext
			{
        TcpReadContext();
        ~TcpReadContext();

				// Returns true if all chunks are received.
				bool isComplete()
				{
					if (currentMessageHeader->ChunkType == 'A')
						return true;
					else if (chunks.empty())
						return false;
					else
					{
						// Even if the current message chunk is final, check if all chunks are received (in case if they came out of order).
						// If we go from the latest chunk to the beginning of the map, then each entry should have sequence number decremented by 1.
						uint32_t lastSequenceNumber = chunks.rbegin()->first;
						uint32_t firstSequenceNumber = chunks.begin()->first;
						if (((size_t) lastSequenceNumber + 1 - (size_t) firstSequenceNumber) == chunks.size() && chunks.rbegin()->second.first->ChunkType == 'F')
							return true;
						else
							return false;
					}
				}

				DataBufferPtr assembleMessage()
				{
					if (chunks.size() <= 1)
						return currentDataBuffer;
					else
					{
						uint32_t totalSize = 0;
						for (auto iter = chunks.begin(); iter != chunks.end(); iter++)
						{
							totalSize += iter->second.second->size();
						}
						currentDataBuffer.reset(new DataBuffer(totalSize));
						for (auto iter = chunks.begin(); iter != chunks.end(); iter++)
						{
							currentDataBuffer->push_back(iter->second.second);
							iter->second.second.reset();
						}
						return currentDataBuffer;
					}
				}

				// Ordered by sequence number message chunks with headers:
				std::map<uint32_t, std::pair<MessageHeaderPtr, DataBufferPtr>> chunks;
				// Number of received chunks:
				uint32_t chunkCounter;
				// Storage for message header of the currently received message chunk:
				MessageHeaderPtr currentMessageHeader;
				// Currently used data Buffer. It is also storage for the final  total message buffer, assembled from multiple chunks.
				DataBufferPtr currentDataBuffer;

        void reset();
			};
			typedef std::shared_ptr<TcpReadContext> TcpReadContextPtr;

    }
  }
