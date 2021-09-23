#pragma once

#include "ConsumeOrderContext.h"
#include "Message.h"
#include "OrderAction.h"

namespace ons {

class MessageOrderListener {
public:
  virtual ~MessageOrderListener() = default;

  // interface of consuming message, should be realized by application
  virtual OrderAction consume(const Message &message,
                              const ConsumeOrderContext &context) = 0;
};
} // namespace ons