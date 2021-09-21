#pragma once

#include "absl/strings/string_view.h"

#include "MessageListener.h"

namespace ons {

class ONSCLIENT_API PushConsumer {
public:
  PushConsumer() = default;

  virtual ~PushConsumer() = default;

  virtual void start() = 0;

  virtual void shutdown() = 0;

  virtual void subscribe(absl::string_view topic, absl::string_view filter_expression, MessageListener* listener) = 0;
};

} // namespace ons