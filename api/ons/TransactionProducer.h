#pragma once

#include "LocalTransactionExecuter.h"
#include "ONSClient.h"
#include "SendResultONS.h"

namespace ons {
class ONSCLIENT_API TransactionProducer {
public:
  TransactionProducer() {}
  virtual ~TransactionProducer() {}

  // before send msg, start must be called to allocate resources.
  virtual void start() = 0;

  // before exit ons, shutdown must be called to release all resources allocated
  // by ons internally.
  virtual void shutdown() = 0;

  // retry max 3 times if send failed. if no exception thrown, it sends
  // success;
  virtual SendResultONS send(Message& msg, LocalTransactionExecuter* executor) = 0;
};
} // namespace ons