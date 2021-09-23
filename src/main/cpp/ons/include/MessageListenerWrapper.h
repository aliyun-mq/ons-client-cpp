#pragma once

#include "ons/MessageListener.h"
#include "rocketmq/MessageListener.h"
#include "rocketmq/RocketMQ.h"

namespace ons {

class MessageListenerWrapper : public ROCKETMQ_NAMESPACE::StandardMessageListener {

public:
  explicit MessageListenerWrapper(ons::MessageListener* message_listener);

  ROCKETMQ_NAMESPACE::ConsumeMessageResult
  consumeMessage(const std::vector<ROCKETMQ_NAMESPACE::MQMessageExt>& msgs) override;

private:
  ons::MessageListener* message_listener_{nullptr};
};

}; // namespace ons