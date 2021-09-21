#pragma once

#include "Message.h"
#include "SendResultONS.h"

namespace ons {

class ONSCLIENT_API OrderProducer {
public:
  OrderProducer() = default;

  virtual ~OrderProducer() = default;

  // before send msg, start must be called to allocate resources.
  virtual void start() = 0;

  // before exit ons, shutdown must be called to release all resources allocated
  // by ons internally.
  virtual void shutdown() = 0;

  virtual SendResultONS send(Message& msg, std::string shardingKey) = 0;
};

} // namespace ons