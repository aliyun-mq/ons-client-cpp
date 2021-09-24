#pragma once
#include <string>

#include "absl/strings/string_view.h"

#include "ONSClient.h"

namespace ons {

class SendCallbackONSWrapper;
class ONSSendCallback;
class ProducerImpl;
class OrderProducerImpl;
class TransactionProducerImpl;

class ONSCLIENT_API SendResultONS {
public:
  SendResultONS();

  virtual ~SendResultONS();

  const std::string &getMessageId() const;

  operator bool() { return !message_id_.empty(); }

protected:
  void setMessageId(absl::string_view message_id);

private:
  std::string message_id_;

  friend class SendCallbackONSWrapper;
  friend class ONSSendCallback;
  friend class ProducerImpl;
  friend class OrderProducerImpl;
  friend class TransactionProducerImpl;
};

} // namespace ons
