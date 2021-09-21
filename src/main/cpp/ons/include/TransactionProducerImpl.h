#pragma once

#include "ons/ONSFactory.h"

#include "ons/TransactionProducer.h"

namespace ons {

class TransactionProducerImpl : public TransactionProducer {
public:
  // TODO: not yet implemented
  TransactionProducerImpl(const ONSFactoryProperty& factory_property, LocalTransactionChecker* checker);

  // TODO: not yet implemented
  ~TransactionProducerImpl() override = default;

  // TODO: not yet implemented
  void start() override;

  // TODO: not yet implemented
  void shutdown() override;

  // TODO: not yet implemented
  SendResultONS send(Message& msg, LocalTransactionExecuter* executor) override;
};

} // namespace ons