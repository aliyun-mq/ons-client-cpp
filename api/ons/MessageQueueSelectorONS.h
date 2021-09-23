#pragma once

#include "Message.h"
#include "MessageQueueONS.h"

namespace ons {

class MessageQueueSelectorONS {
public:
  virtual ~MessageQueueSelectorONS() = default;
  
  virtual MessageQueueONS select(const std::vector<MessageQueueONS>& mqs, const Message& msg, void* arg) = 0;
};

} // namespace ons