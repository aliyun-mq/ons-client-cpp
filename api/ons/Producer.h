#pragma once

#include <system_error>

#include "Message.h"
#include "MessageQueueONS.h"
#include "ONSCallback.h"
#include "ONSClientException.h"
#include "SendResultONS.h"

namespace ons {

class ONSCLIENT_API Producer {
public:
  virtual ~Producer() = default;

  // before sending msg, start function must be called to prepare and allocate
  // necessary resources.
  virtual void start() = 0;

  // shutdown must be called to release all internally allocated resources
  // before quit.
  virtual void shutdown() = 0;

  // retry at most 3 times internally. Sending can be regarded as success if no
  // exception is raised.

  /**
   * @brief Delivery message to brokers. By default, 3-times-retry policy are
   * employed on sending failure. If SDK still failed after all retry quota are
   * exchausted, ONSClientException will be raised and application developers
   * are supposed to handle such situation.
   *
   * @param message
   * @return ons::SendResultONS
   * @throw ONSClientException
   */
  virtual ons::SendResultONS send(Message& message) noexcept(false) = 0;

  /**
   * @brief Send message asynchronously.
   *
   * @param msg
   * @param callback
   */
  virtual void sendAsync(Message& message, ons::SendCallbackONS* callback) noexcept = 0;

  // one-way send
  virtual void sendOneway(Message& message) noexcept = 0;
};

} // namespace ons