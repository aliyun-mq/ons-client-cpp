#pragma once

#include "Message.h"
#include "TransactionStatus.h"

namespace ons {
class LocalTransactionExecutor {
public:
  virtual ~LocalTransactionExecutor() = default;

  virtual TransactionStatus execute(const Message& msg) noexcept = 0;
};

// Keep API compatible
using LocalTransactionExecuter = LocalTransactionExecutor;

} // namespace ons