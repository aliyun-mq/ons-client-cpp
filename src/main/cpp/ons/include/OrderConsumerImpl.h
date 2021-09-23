#pragma once

#include <memory>

#include "ONSConsumerAbstract.h"
#include "OrderListenerWrapper.h"
#include "ons/ONSFactory.h"
#include "ons/OrderConsumer.h"

#include "rocketmq/DefaultMQPushConsumer.h"

namespace ons {

class OrderConsumerImpl : public OrderConsumer, public ONSConsumerAbstract {
public:
  explicit OrderConsumerImpl(const ONSFactoryProperty& factory_property);

  ~OrderConsumerImpl() override = default;

  void start() override;

  void shutdown() override;

  void subscribe(absl::string_view topic, absl::string_view subscribe_expression) override;

  void registerMessageListener(MessageOrderListener* listener) override;
};

} // namespace ons