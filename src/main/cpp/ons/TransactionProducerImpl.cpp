#include "TransactionProducerImpl.h"
#include "ons/SendResultONS.h"

namespace ons {

// TODO: not yet implemented
TransactionProducerImpl::TransactionProducerImpl(const ONSFactoryProperty& ons_factory_property,
                                                 LocalTransactionChecker* checker)
    : ONSClientAbstract(ons_factory_property), producer_(ons_factory_property.getGroupId()) {}

void TransactionProducerImpl::start() { producer_.start(); }

void TransactionProducerImpl::shutdown() { producer_.shutdown(); }

SendResultONS TransactionProducerImpl::send(Message& msg, LocalTransactionExecuter* executor) {
  return SendResultONS();
}

} // namespace ons