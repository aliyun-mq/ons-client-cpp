#pragma once

#include "Action.h"
#include "ConsumeContext.h"
#include "Message.h"

namespace ons {

class ONSCLIENT_API MessageListener {
public:
  MessageListener() = default;

  virtual ~MessageListener() = default;

  // interface of consuming message, should be realized by application
  virtual Action consume(Message& message, ConsumeContext& context) = 0;
};
} // namespace ons