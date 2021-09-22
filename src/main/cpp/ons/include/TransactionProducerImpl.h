#pragma once

#include "ons/TransactionProducer.h"

#include "ONSClientAbstract.h"
#include "ons/ONSFactory.h"
#include "rocketmq/DefaultMQProducer.h"

namespace ons {

class TransactionProducerImpl : public TransactionProducer, public ONSClientAbstract {
public:
  TransactionProducerImpl(const ONSFactoryProperty& factory_property, LocalTransactionChecker* checker);

  ~TransactionProducerImpl() override = default;

  void start() override;

  void shutdown() override;

  SendResultONS send(Message& msg, LocalTransactionExecuter* executor) override;

private:
  ROCKETMQ_NAMESPACE::DefaultMQProducer producer_;
};

} // namespace ons