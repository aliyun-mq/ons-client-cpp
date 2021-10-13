#pragma once

#include <string>

#include "MessageOrderListener.h"

namespace ons {

class ONSCLIENT_API OrderConsumer {
public:
  virtual ~OrderConsumer() = default;

  virtual void start() = 0;

  virtual void shutdown() = 0;

  virtual void subscribe(const std::string& topic, const std::string& expression) = 0;

  virtual void registerMessageListener(MessageOrderListener* listener) = 0;
};

} // namespace ons