#pragma once

#include "ons/ONSFactory.h"
#include "ons/OrderConsumer.h"

namespace ons {

class OrderConsumerImpl : public OrderConsumer {
public:
  explicit OrderConsumerImpl(const ONSFactoryProperty& factory_property);

  ~OrderConsumerImpl() override = default;

  void start() override;

  void shutdown() override;

  void subscribe(const char* topic, const char* subscribe_expression) override;

  void registerMessageListener(MessageOrderListener* listener) override;
};

} // namespace ons