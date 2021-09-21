#pragma once

#include "Message.h"
#include "MessageQueueONS.h"
#include "ONSCallback.h"
#include "ONSClientException.h"
#include "SendResultONS.h"

namespace ons {

class ONSCLIENT_API Producer {
public:
  Producer() = default;

  virtual ~Producer() = default;

  // before sending msg, start function must be called to prepare and allocate necessary resources.
  virtual void start() = 0;

  // shutdown must be called to release all internally allocated resources before quit.
  virtual void shutdown() = 0;

  // retry at most 3 times internally. Sending can be regarded as success if no exception is raised.
  virtual ons::SendResultONS send(Message& message) = 0;

  virtual ons::SendResultONS send(Message& message, const MessageQueueONS& mq) = 0;

  // async send
  virtual void sendAsync(Message& msg, ons::SendCallbackONS* callback) = 0;

  // one-way send
  virtual void sendOneway(Message& msg) = 0;
};

} // namespace ons