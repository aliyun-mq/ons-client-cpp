#pragma once

#include "ons/ONSFactory.h"
#include "ons/OrderProducer.h"

namespace ons {

class OrderProducerImpl : public OrderProducer {

public:
  // TODO: not yet implemented
  explicit OrderProducerImpl(const ONSFactoryProperty& factory_property);

  // TODO: not yet implemented
  ~OrderProducerImpl() override = default;

  // TODO: not yet implemented
  void start() override;

  // TODO: not yet implemented
  void shutdown() override;

  // TODO: not yet implemented
  SendResultONS send(Message& msg, std::string sharding_key) override;
};

} // namespace ons