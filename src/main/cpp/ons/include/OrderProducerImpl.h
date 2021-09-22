#pragma once

#include "ONSClientAbstract.h"
#include "ONSMessageQueueSelector.h"
#include "ons/ONSFactory.h"
#include "ons/OrderProducer.h"
#include "rocketmq/DefaultMQProducer.h"

namespace ons {

class OrderProducerImpl : public OrderProducer, public ONSClientAbstract {

public:
  explicit OrderProducerImpl(const ONSFactoryProperty& factory_property);

  ~OrderProducerImpl() override = default;

  void start() override;

  void shutdown() override;

  SendResultONS send(Message& msg, std::string message_group) override;

private:
  ROCKETMQ_NAMESPACE::DefaultMQProducer producer_;
  ONSMessageQueueSelector selector_;
};

} // namespace ons