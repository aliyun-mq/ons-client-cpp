#pragma once

#include "Message.h"
#include "TransactionStatus.h"

namespace ons {

class LocalTransactionChecker {
public:
  virtual ~LocalTransactionChecker() = default;

  virtual TransactionStatus check(const Message& msg) noexcept = 0;
};

} // namespace ons
