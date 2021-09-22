#pragma once

#include "Message.h"
#include "TransactionStatus.h"

namespace ons {

class LocalTransactionChecker {
public:
  LocalTransactionChecker() = default;


  virtual ~LocalTransactionChecker() = default;

  virtual TransactionStatus check(Message &msg) = 0;
};

} // namespace ons
