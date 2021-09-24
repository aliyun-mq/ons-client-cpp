#pragma once

#include "Action.h"
#include "ConsumeContext.h"
#include "Message.h"

namespace ons {

class ONSCLIENT_API MessageListener {
public:
  virtual ~MessageListener() = default;

  virtual Action consume(const Message& message, ConsumeContext& context) noexcept = 0;
};
} // namespace ons