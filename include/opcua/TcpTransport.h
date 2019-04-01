#pragma once
#ifndef owa_tcp_transport_h
#define owa_tcp_transport_h

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <memory>
#include "opcua/MessageHeader.h"
#include "opcua/BinaryCodec.h"
#include "opcua/Transport.h"
#include "opcua/CallbackFunctionDefs.h"
#include <mutex>
#include <atomic>
#include "opcua/SymmetricCryptoContext.h"
#include <atomic>
#include <iostream>
#include <queue>
#include "opcua/TransportSettings.h"
#include <map>

namespace OWA {
  namespace OpcUa {
    class TcpTransport : public Transport {
    public:
      TcpTransport(std::shared_ptr<Codec> codec, std::shared_ptr<Cryptor> cryptor);
      virtual ~TcpTransport();
			void setConfiguration(const TransportSettings& config);
			TransportSettings getConfiguration() const;

			void setSelfRef(std::weak_ptr<TcpTransport>& ref);
			std::weak_ptr<Transport> getSelfRef();
      void setCallback(std::shared_ptr<connectionStateChangeCallback>& f) { connectCallback = f; }
      void setCallback(std::shared_ptr<onResponseReceived<FindServersResponse>>& f) { onFindServersResponse = f; }
      void setCallback(std::shared_ptr<onResponseReceived<OpenSecureChannelResponse>>& f) { onOpenSecureChannelResponse = f; }
      void setCallback(std::shared_ptr<onResponseReceived<GetEndpointsResponse>>& f) { onGetEndpointsResponse = f; }
      void setCallback(std::shared_ptr<onResponseReceived<CloseSecureChannelResponse>>& f) { onCloseSecureChannelResponse = f; }
      void setCallback(std::shared_ptr<onResponseReceived<ServiceFaultResponse>>& f) { onServiceFaultResponse = f; }

      void setCallback(std::shared_ptr<onResponseReceived<CreateSessionResponse>>& f);
      void setCallback(std::shared_ptr<onResponseReceived<ActivateSessionResponse>>& f);
			void setCallback(std::shared_ptr<onResponseReceived<CloseSessionResponse>>& f);

      void setCallback(std::shared_ptr<onResponseReceived<BrowseResponse>>& f);
      void setCallback(std::shared_ptr<onResponseReceived<BrowseNextResponse>>& f);
      void setCallback(std::shared_ptr<onResponseReceived<TranslateBrowsePathsToNodeIdsResponse>>& f);

      void setCallback(std::shared_ptr<onResponseReceived<ReadResponse>>& f);
      void setCallback(std::shared_ptr<onResponseReceived<WriteResponse>>& f);
      void setCallback(std::shared_ptr<onResponseReceived<HistoryReadResponse>>& f);
      void setCallback(std::shared_ptr<onResponseReceived<HistoryUpdateResponse>>& f);

      void setCallback(std::shared_ptr<onResponseReceived<CreateSubscriptionResponse>>& f);
      void setCallback(std::shared_ptr<onResponseReceived<ModifySubscriptionResponse>>& f);
      void setCallback(std::shared_ptr<onResponseReceived<DeleteSubscriptionsResponse>>& f);
      void setCallback(std::shared_ptr<onResponseReceived<TransferSubscriptionsResponse>>& f);
      void setCallback(std::shared_ptr<onResponseReceived<SetPublishingModeResponse>>& f);
      void setCallback(std::shared_ptr<onResponseReceived<PublishResponse>>& f);
      void setCallback(std::shared_ptr<onResponseReceived<RepublishResponse>>& f);

      void setCallback(std::shared_ptr<onResponseReceived<CreateMonitoredItemsResponse>>& f);
      void setCallback(std::shared_ptr<onResponseReceived<ModifyMonitoredItemsResponse>>& f);
      void setCallback(std::shared_ptr<onResponseReceived<DeleteMonitoredItemsResponse>>& f);
      void setCallback(std::shared_ptr<onResponseReceived<SetTriggeringResponse>>& f);
      void setCallback(std::shared_ptr<onResponseReceived<SetMonitoringModeResponse>>& f);

      void setCallback(std::shared_ptr<onResponseReceived<CallResponse>>& f);
      
      ////****************************************/

      void sendRequest(std::shared_ptr<OpenSecureChannelRequest>& request);
      void sendRequest(std::shared_ptr<FindServersRequest>& request);
      void sendRequest(std::shared_ptr<GetEndpointsRequest>& request);
      void sendRequest(std::shared_ptr<CloseSecureChannelRequest>& request);

      void sendRequest(std::shared_ptr<CreateSessionRequest>& request);
      void sendRequest(std::shared_ptr<ActivateSessionRequest>& request);
			void sendRequest(std::shared_ptr<CloseSessionRequest>& request);

      void sendRequest(std::shared_ptr<BrowseRequest>& request);
      void sendRequest(std::shared_ptr<BrowseNextRequest>& request);
      void sendRequest(std::shared_ptr<TranslateBrowsePathsToNodeIdsRequest>& request);

      void sendRequest(std::shared_ptr<ReadRequest>& request);
      void sendRequest(std::shared_ptr<WriteRequest>& request);
      void sendRequest(std::shared_ptr<HistoryReadRequest>& request);
      void sendRequest(std::shared_ptr<HistoryUpdateRequest>& request);

      void sendRequest(std::shared_ptr<CreateSubscriptionRequest>& request);
      void sendRequest(std::shared_ptr<ModifySubscriptionRequest>& request);
      void sendRequest(std::shared_ptr<DeleteSubscriptionsRequest>& request);
      void sendRequest(std::shared_ptr<TransferSubscriptionsRequest>& request);
      void sendRequest(std::shared_ptr<SetPublishingModeRequest>& request);
      void sendRequest(std::shared_ptr<PublishRequest>& request);
      void sendRequest(std::shared_ptr<RepublishRequest>& request);

      void sendRequest(std::shared_ptr<CreateMonitoredItemsRequest>& request);
      void sendRequest(std::shared_ptr<ModifyMonitoredItemsRequest>& request);
      void sendRequest(std::shared_ptr<DeleteMonitoredItemsRequest>& request);
      void sendRequest(std::shared_ptr<SetMonitoringModeRequest>& request);
      void sendRequest(std::shared_ptr<SetTriggeringRequest>& request);

      void sendRequest(std::shared_ptr<CallRequest>& request);

	    /**
	    * Asynchronously starts connection attempt. If timeout is less than 10 ms, sets it to 10 ms.
	    */
      void connect(const std::string& endpointUrl, uint32_t timeoutInMilliseconds = 5000);
	    
      virtual void disconnect(bool doCallBack = true);
      virtual void listen(const boost::any& context, const std::string& url);
      virtual void stopListen(const boost::any& context);
      
      void AddSymmetricCryptoContext(std::shared_ptr<SymmetricCryptoContext>& context);
      void RemoveSymmetricCryptoContext(uint32_t tokenId);
      std::shared_ptr<SymmetricCryptoContext> GetSymmetricCryptoContext(uint32_t tokenId);
			std::string getEndpointUrl();

			void sendChunkIfFull();
			void checkTotalMessageSize(uint32_t bytesToSend);
			uint32_t getMaxSendChunkCount();
			void setTransportSettings(const TransportSettings& settings);
    protected:
      // Template member functions cannot be virtual, as a workaround created methods with explicit types which then call this method:
      template<typename RequestType>
      void sendRequestImpl(std::shared_ptr<RequestType>& request);

      void start_connect(boost::asio::ip::tcp::resolver::iterator endpoint_iter);
      void handle_connect(boost::asio::ip::tcp::resolver::iterator endpoint_iter, const boost::system::error_code& ec);
	    void handle_connect_timeout();
	    
      void start_read_message_header();
      void handle_read_message_header(TcpReadContextPtr readContext, const boost::system::error_code& ec, size_t bytes);

      void handle_read_message_body(TcpReadContextPtr readContext, const boost::system::error_code& ec, size_t bytes);

			bool isOk(const boost::system::error_code& ec, size_t bytes);
      void stop();

      void start_send_hello();
      void handle_hello(DataBufferPtr buffer, const boost::system::error_code& ec, size_t bytes);
			void handleChannelClose(uint32_t id, DataBufferPtr buffer, const boost::system::error_code& ec, size_t bytes);


      //void start_open_secure_channel();
      //void handle_open_secure_channel(const boost::system::error_code& ec);

      //void start_send_message();
      //void handle_send_message(const boost::system::error_code& ec);

	    std::atomic<ConnectionState> state;
      std::atomic<bool> stopped_;
      std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
	    std::shared_ptr<boost::asio::deadline_timer> timer_;
      std::shared_ptr<boost::asio::io_service> io_service;
      std::shared_ptr<boost::asio::ip::tcp::resolver> resolver;

      std::shared_ptr<OWA::OpcUa::Codec> codec;
      std::shared_ptr<Cryptor> cryptor;

      DataBufferPtr messageCache;

      std::string endpointUrl;
	    uint32_t connectionTimeout;
      int numberOfAckMessagesReceived;

      std::shared_ptr<connectionStateChangeCallback> connectCallback;
      std::shared_ptr<onResponseReceived<OpenSecureChannelResponse>> onOpenSecureChannelResponse;
      std::shared_ptr<onResponseReceived<FindServersResponse>> onFindServersResponse;
      std::shared_ptr<onResponseReceived<GetEndpointsResponse>> onGetEndpointsResponse;
      std::shared_ptr<onResponseReceived<CloseSecureChannelResponse>> onCloseSecureChannelResponse;
      std::shared_ptr<onResponseReceived<ServiceFaultResponse>> onServiceFaultResponse;

      std::shared_ptr<onResponseReceived<CreateSessionResponse>> onCreateSessionResponse;
      std::shared_ptr<onResponseReceived<ActivateSessionResponse>> onActivateSessionResponse;
			std::shared_ptr<onResponseReceived<CloseSessionResponse>> onCloseSessionResponse;

      std::shared_ptr<onResponseReceived<BrowseResponse>> onBrowseResponse;
      std::shared_ptr<onResponseReceived<BrowseNextResponse>> onBrowseNextResponse;
      std::shared_ptr<onResponseReceived<TranslateBrowsePathsToNodeIdsResponse>> onTranslateBrowsePathsToNodeIdsResponse;

      std::shared_ptr<onResponseReceived<ReadResponse>> onReadResponse;
      std::shared_ptr<onResponseReceived<HistoryReadResponse>> onHistoryReadResponse;
      std::shared_ptr<onResponseReceived<WriteResponse>> onWriteResponse;
      std::shared_ptr<onResponseReceived<HistoryUpdateResponse>> onHistoryUpdateResponse;

      std::shared_ptr<onResponseReceived<CreateSubscriptionResponse>> onCreateSubscriptionResponse;
      std::shared_ptr<onResponseReceived<ModifySubscriptionResponse>> onModifySubscriptionResponse;
      std::shared_ptr<onResponseReceived<DeleteSubscriptionsResponse>> onDeleteSubscriptionsResponse;
      std::shared_ptr<onResponseReceived<TransferSubscriptionsResponse>> onTransferSubscriptionsResponse;
      std::shared_ptr<onResponseReceived<SetPublishingModeResponse>> onSetPublishingModeResponse;
      std::shared_ptr<onResponseReceived<PublishResponse>> onPublishResponse;
      std::shared_ptr<onResponseReceived<RepublishResponse>> onRepublishResponse;
      
      std::shared_ptr<onResponseReceived<CreateMonitoredItemsResponse>> onCreateMonitoredItemsResponse;
      std::shared_ptr<onResponseReceived<ModifyMonitoredItemsResponse>> onModifyMonitoredItemsResponse;
      std::shared_ptr<onResponseReceived<DeleteMonitoredItemsResponse>> onDeleteMonitoredItemsResponse;
      std::shared_ptr<onResponseReceived<SetMonitoringModeResponse>> onSetMonitoringModeResponse;
      std::shared_ptr<onResponseReceived<SetTriggeringResponse>> onSetTriggeringResponse;

      std::shared_ptr<onResponseReceived<CallResponse>> onCallResponse;

      std::map<uint32_t, std::shared_ptr<SymmetricCryptoContext>> symmetricCryptoContexts;
			
			// security token ID used to send messages. It is the one which is returned in the latest OpenSecureChannel response form the server.
			ChannelSecurityToken currentSendToken;

      std::mutex sendMutex;
      ByteString clientNonce;

			std::weak_ptr<TcpTransport> selfRef;

			bool readyToSend;
			std::condition_variable sendConditionVariable;
			
			// Data Buffer which is currently being sent.
			DataBufferPtr outgoingDataBuffer;

			TransportSettings requestedSettings;
			TransportSettings revisedSettings;
    };

    template<typename RequestType>
    void OWA::OpcUa::TcpTransport::sendRequestImpl(std::shared_ptr<RequestType>& request)
    {
      // Encode and put into the queue from within a lock, so only one thread at a time can do that, to make sure
      // that messages are sent in order with sequence numbers.
        
      // Encode it:
			DataBufferPtr message = std::make_shared<DataBuffer>();
			outgoingDataBuffer = message;
			std::shared_ptr<Transport> r = selfRef.lock();

			message->setTransport(r);
			message->setSecurityRequestId(request->header.getRequestId());

			// Wait until previous request is sent:
			std::unique_lock<std::mutex> lk(sendMutex);
			sendConditionVariable.wait(lk, [this] 
			{
				return this->readyToSend;
			});

			if (stopped_ || !socket_ || state != Connected)
			{
				readyToSend = true;
				lk.unlock();
				throw OperationResult(StatusCode::BadDisconnect, "Transport layer is not connected");
			}
			readyToSend = false;
      auto context = GetSymmetricCryptoContext(currentSendToken.TokenId);
      codec->encode(message, *request, context);

			std::function<OperationResult(std::weak_ptr<TcpTransport> wp, DataBufferPtr& buffer)> sendFunction;

      // Start an asynchronous operation to send the request message:
			try {
				if (request->getTypeId() == RequestResponseTypeId::CloseSecureChannelRequest) 
				{
					std::weak_ptr<TcpTransport> wp = selfRef;
					boost::asio::async_write(*socket_, boost::asio::buffer(message->data(), message->size()), [wp, request, message]
					(const boost::system::error_code& ec, size_t bytes)
					{
						if (auto sp = wp.lock())
						{
							// No response comes from the server for this request, so requestId needs to be passed to the callback:
							sp->handleChannelClose(request->header.requestId, message, ec, bytes);
							{
								std::lock_guard<std::mutex> lk(sp->sendMutex);
								sp->readyToSend = true;
							}
							sp->sendConditionVariable.notify_one();
						}
					});
				}
				else 
				{
					// Send message
					std::weak_ptr<TcpTransport> wp = selfRef;
					boost::asio::async_write(*socket_, boost::asio::buffer(message->data(), message->size()), [wp, message](const boost::system::error_code& ec, size_t bytes)
					{
						if (auto sp = wp.lock())
						{
							sp->handle_hello(message, ec, bytes);
							{
								std::lock_guard<std::mutex> lk(sp->sendMutex);
								sp->readyToSend = true;
							}
							sp->sendConditionVariable.notify_one();
						}
					});
				}
			}
			catch (const OperationResult& or )
			{
				std::cout << "TcpTransport::sendRequest caught expection: " << or.toString() << std::endl;
				readyToSend = true;
				lk.unlock();
				sendConditionVariable.notify_one();
				throw or ;
			}
			catch (const std::exception& ex) 
			{
				readyToSend = true;
				lk.unlock();
				sendConditionVariable.notify_one();
				throw OperationResult(StatusCode::BadCommunicationError, ex.what());
			}

			// Unlock mutex
			lk.unlock();
    }

  }
}

#endif