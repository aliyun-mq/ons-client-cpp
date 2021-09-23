#pragma once

#include "MessageListenerWrapper.h"
#include "ONSConsumerAbstract.h"
#include "absl/container/flat_hash_map.h"
#include "ons/ONSFactory.h"
#include "rocketmq/DefaultMQPushConsumer.h"

namespace ons {

class ConsumerImpl : public PushConsumer, public ONSConsumerAbstract {
public:
  explicit ConsumerImpl(const ONSFactoryProperty& factory_property);

  ~ConsumerImpl() override = default;

  void start() override;

  void shutdown() override;

  void subscribe(absl::string_view topic, absl::string_view sub_expression) override;

  void registerMessageListener(MessageListener* listener) override;
};

} // namespace ons