#pragma once

#include "ons/ONSFactory.h"

#include "ons/OrderConsumer.h"

namespace ons {

class OrderConsumerImpl : public OrderConsumer {
public:
  // TODO: not yet implemented
  explicit OrderConsumerImpl(const ONSFactoryProperty& ons_factory_property);
  // TODO: not yet implemented
  ~OrderConsumerImpl() override = default;
  // TODO: not yet implemented
  void start() override;
  // TODO: not yet implemented
  void shutdown() override;
  // TODO: not yet implemented
  void subscribe(const char* topic, const char* subscribe_expression, MessageOrderListener* listener) override;
};
} // namespace ons