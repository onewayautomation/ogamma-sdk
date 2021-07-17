#pragma once
#include <memory>
#include <atomic>
#include "opcua/ConnectDisconnectContext.h"

namespace OWA {
  namespace OpcUa {

    // CallbackHanlder is intended to control callbacks to the parent object from member object, 
    // making sure that the child object does not make callbacks when parent is destroyed or begin destroyed,
    // or still exists but temporarily does not want to handle callbacks.
    // The child object should have either a method to set weak pointer to this object, or accept it as argument in the constructor.
    // And then do make callbacks only after getting strong pointer and having value 'true' returned after calling its canCallBack method.
    // The child object must not destroy received strong pointer until the callback returns. This makes sure that the parent
    // object is not destroyed while there is any callback in progress.
    // The first use case is Connection and Transport relations.
    class LifeTimeWatch : public std::enable_shared_from_this<LifeTimeWatch>
    {
    public:
      LifeTimeWatch() : std::enable_shared_from_this<LifeTimeWatch>() 
      {
        valid.store(true);
      }
      ~LifeTimeWatch() {
        if (transaction && !transaction->completed())
        {
          // transaction->complete();
        }
      }

      inline void setValid(bool newValue)
      {
        valid = newValue;
      }
      inline bool isValid()
      {
        return valid;
      }

      void setTransaction(std::shared_ptr<ConnectionDisconnectContext>& transaction_)
      {
        if (transaction && !transaction->completed())
          transaction->complete();

        this->transaction = transaction_;
      }

      inline void onCallbackComplete() 
      {
        auto s = this->shared_from_this(); // This adds 1 more strong reference 

        if (  s.use_count() <= (1 + 1) // 1 reference in 's', and 1 in the LifeTimeWatch
          || (!valid && s.use_count() <= 3)) // The third is owned by Connection, but should not be counted 
        {
          // The result must be set before completing of the transaction.
          // Otherwise, once the Disconnect transaction is created, it can be completed when OnCloseSession callback is finished.
          if (transaction && transaction->resultSet())
          {
            // Completion of the transaction can delete this object. Therefore, use copy.
            auto trCopy = transaction;
            transaction.reset();
            s.reset();
            
            if (!trCopy->completed())
              trCopy->complete();

            trCopy.reset();
          }
        }
      }
    protected:
      std::shared_ptr<ConnectionDisconnectContext> transaction;
      std::atomic<bool> valid;
    };

    template<typename VariableType>
    class VariableChanger {
    public:
      VariableChanger(VariableType* valuePtr_, const VariableType& value_) {
        valuePtr = valuePtr_;
        value = value_;
      }
      ~VariableChanger() {
        *valuePtr = value;
      }
    protected:
      VariableType* valuePtr;
      VariableType value;
    };
  }
}
