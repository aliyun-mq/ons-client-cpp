#include "TransactionProducerImpl.h"
#include "ons/SendResultONS.h"

namespace ons {

// TODO: not yet implemented
TransactionProducerImpl::TransactionProducerImpl(const ONSFactoryProperty& ons_factory_property,
                                                 LocalTransactionChecker* checker) {}

// TODO: not yet implemented
void TransactionProducerImpl::start() {}

// TODO: not yet implemented
void TransactionProducerImpl::shutdown() {}

// TODO: not yet implemented
SendResultONS TransactionProducerImpl::send(Message& msg, LocalTransactionExecuter* executor) {
  return SendResultONS();
}

} // namespace ons