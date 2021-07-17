#pragma once

#include "opcua/OperationResult.h"
#include "opcua/Enums.h"
#include <functional>
#include <future>
#include <mutex>
#include "opcua/DateTime.h"
#include <memory>

namespace OWA {
  namespace OpcUa {
    class Connection;
    typedef std::function <void(std::weak_ptr<Connection>, const OperationResult& result)> generalCallback;

    enum class TransactionState:uint8_t {
      New = 0,
      Deferred = 1,
      InProgress = 2,
      Completed = 3
    };

    struct ConnectionDisconnectContext: public std::enable_shared_from_this<ConnectionDisconnectContext>
    {
      ConnectionDisconnectContext(const ConnectionDisconnectContext&) = delete;
      ConnectionDisconnectContext& operator=(const ConnectionDisconnectContext&) = delete;

      ConnectionDisconnectContext(
        std::weak_ptr<Connection> connection_,
        generalCallback callback_, ConnectionState nextState_
      );
      ~ConnectionDisconnectContext();

      ConnectionDisconnectContext& defer();
      bool isDeferred();
      bool addCallback(generalCallback callback_);

      // If calling callback functions is not desirable, clear them.
      ConnectionDisconnectContext& clearCallbacks();

      ConnectionDisconnectContext& setResult(const OperationResult& operationResult, bool overWrite = false);
      bool resultSet();

      ConnectionDisconnectContext& setState(ConnectionState newState, bool overWrite = false);
      ConnectionDisconnectContext& setResultAndState(const OperationResult& operationResult, ConnectionState newState, bool overWrite = false);
      std::shared_future<OperationResult> complete();
      
      uint32_t getAgeInMs();

      // Set function to call before completion.
      bool setPreAction(std::function<void()>& nextAction);

      // Set function to call after completion.
      bool setNextAction(std::function<void()>& nextAction);

      bool completed();
      TransactionState getState();
      
      std::shared_future<OperationResult> getFuture();

      ConnectionState getDesiredState();
       
      OperationResult getResult();

    protected:
      std::promise<OperationResult> promise;
      std::vector<generalCallback> callbacks;
      std::shared_future<OperationResult> future;
      // NextState defines transaction type: if Connected, then it is connect transaction, otherwise, disconnect transaction.
      ConnectionState nextState;
      ConnectionState actualState;
      OperationResult result;

      // Function to call after this transaction is complete.
      std::function<void()> nextAction;
      bool hasNextAction;

      std::function<void()> preAction;
      bool hasPreAction;

      std::recursive_mutex stateMutex;

      // Flags to prevent  setting result or state multiple times.
      bool isResultSet;
      bool isStateSet;

      DateTime createdTime;
      TransactionState ownState;
      std::weak_ptr<Connection> connection;
    };
  }
}