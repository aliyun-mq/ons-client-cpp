#pragma once

#include "MessageListener.h"

namespace ons {

class ONSCLIENT_API PushConsumer {
public:
  PushConsumer() = default;

  virtual ~PushConsumer() = default;

  virtual void start() = 0;

  virtual void shutdown() = 0;

  virtual void subscribe(const std::string& topic, const std::string& filter_expression) = 0;

  virtual void registerMessageListener(MessageListener* listener) = 0;
};

} // namespace ons