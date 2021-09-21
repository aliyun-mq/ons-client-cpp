#pragma once

#include "Message.h"
#include "TransactionStatus.h"

namespace ons {
class LocalTransactionChecker {
public:
  LocalTransactionChecker() {}
  virtual TransactionStatus check(Message& msg) = 0;
  virtual ~LocalTransactionChecker() {}
};
} // namespace ons
