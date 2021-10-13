#include "ons/SendResultONS.h"

#include <string>

namespace ons {

SendResultONS::SendResultONS() = default;

SendResultONS::~SendResultONS() = default;

void SendResultONS::setMessageId(const std::string& message_id) {
  message_id_ = std::string(message_id.data(), message_id.length());
}

const std::string& SendResultONS::getMessageId() const {
  return message_id_;
}

} // namespace ons
