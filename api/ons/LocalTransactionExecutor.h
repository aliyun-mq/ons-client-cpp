#pragma once
#include "Message.h"
#include "TransactionStatus.h"

namespace ons {
class LocalTransactionExecutor {
public:
  LocalTransactionExecutor() {}
  virtual TransactionStatus execute(Message& msg) = 0;
  virtual ~LocalTransactionExecutor() {}
};

// Keep API compatible
typedef LocalTransactionExecutor LocalTransactionExecuter;

} // namespace ons