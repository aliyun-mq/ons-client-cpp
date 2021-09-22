#pragma once
#include "Message.h"
#include "TransactionStatus.h"

namespace ons {
class LocalTransactionExecutor {
public:
  LocalTransactionExecutor() = default;

  virtual ~LocalTransactionExecutor() = default;

  virtual TransactionStatus execute(const Message &msg) = 0;
};

// Keep API compatible
typedef LocalTransactionExecutor LocalTransactionExecuter;

} // namespace ons