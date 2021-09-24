#pragma once

#include "ConsumeOrderContext.h"
#include "Message.h"
#include "OrderAction.h"

namespace ons {

class MessageOrderListener {
public:
  virtual ~MessageOrderListener() = default;

  virtual OrderAction consume(const Message& message, const ConsumeOrderContext& context) noexcept = 0;
};
} // namespace ons