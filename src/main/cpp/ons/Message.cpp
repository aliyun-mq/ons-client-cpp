#include "ons/Message.h"

#include <chrono>
#include <cstdint>
#include <string>

#include "absl/strings/numbers.h"
#include "absl/strings/str_join.h"
#include "absl/time/time.h"

namespace ons {

const char* SystemPropKey::TAG = "__TAG";
const char* SystemPropKey::KEY_SEPARATOR = " ";
const char* SystemPropKey::MSGID = "__MSGID";
const char* SystemPropKey::RECONSUMETIMES = "__RECONSUMETIMES";
const char* SystemPropKey::STARTDELIVERTIME = "__STARTDELIVERTIME";

Message::Message(absl::string_view topic, absl::string_view body) {
  topic_ = std::string(topic.data(), topic.length());
  body_ = std::string(body.data(), body.length());
}

Message::Message(absl::string_view topic, absl::string_view tag, absl::string_view body) : Message(topic, body) {
  if (!tag.empty()) {
    setTag(tag);
  }
}

Message::Message(absl::string_view topic, absl::string_view tag, absl::string_view key, absl::string_view body)
    : Message(topic, tag, body) {
  if (!key.empty()) {
    attachKey(key);
  }
}

void Message::putSystemProperty(absl::string_view key, absl::string_view value) {
  if (key.empty() || value.empty()) {
    return;
  }

  std::string k(key.data(), key.length());
  std::string v(value.data(), value.length());

  auto search = system_properties_.find(k);
  if (system_properties_.end() != search) {
    system_properties_.erase(search);
  }

  system_properties_.insert({k, v});
}

std::string Message::getSystemProperty(absl::string_view key) const {
  std::string k(key.data(), key.length());
  auto it = system_properties_.find(k);
  if (system_properties_.end() != it) {
    return it->second;
  }
  return std::string();
}

void Message::setSystemProperties(const std::map<std::string, std::string>& system_properties) {
  for (const auto& it : system_properties) {
    putSystemProperty(it.first, it.second);
  }
}

std::map<std::string, std::string> Message::getSystemProperties() const { return system_properties_; }

void Message::putUserProperty(absl::string_view key, absl::string_view value) {
  std::string k(key.data(), key.length());
  std::string v(value.data(), value.length());

  auto search = user_properties_.find(k);
  if (user_properties_.end() != search) {
    if (search->second == v) {
      return;
    }
    user_properties_.erase(search);
  }
  user_properties_.insert({k, v});
}

std::string Message::getUserProperty(absl::string_view key) const {
  std::string k(key.data(), key.length());
  auto it = user_properties_.find(k);
  if (user_properties_.end() != it) {
    return it->second;
  }
  return std::string();
}

void Message::setUserProperties(const std::map<std::string, std::string>& user_properties) {
  for (const auto& it : user_properties) {
    putUserProperty(it.first, it.second);
  }
}

std::map<std::string, std::string> Message::getUserProperties() const { return user_properties_; }

std::string Message::getTopic() const { return topic_; }

void Message::setTopic(absl::string_view topic) {
  if (topic.empty()) {
    return;
  }
  topic_ = std::string(topic.data(), topic.length());
}

std::string Message::getTag() const { return getSystemProperty(SystemPropKey::TAG); }

void Message::setTag(absl::string_view tag) {
  if (tag.empty()) {
    return;
  }

  putSystemProperty(SystemPropKey::TAG, tag);
}

std::string Message::getMsgID() const { return getSystemProperty(SystemPropKey::MSGID); }

void Message::setMsgID(absl::string_view message_id) {
  if (message_id.empty()) {
    return;
  }
  putSystemProperty(SystemPropKey::MSGID, message_id);
}

std::vector<std::string> Message::getKeys() const { return keys_; }

void Message::attachKey(absl::string_view key) {
  if (key.empty()) {
    return;
  }

  keys_.push_back(std::string(key.data(), key.length()));
}

absl::optional<std::chrono::system_clock::time_point> Message::getStartDeliverTime() const {
  std::string&& ms_since_epoch = getSystemProperty(SystemPropKey::STARTDELIVERTIME);
  if (ms_since_epoch.empty()) {
    return absl::make_optional<std::chrono::system_clock::time_point>();
  }

  std::int64_t millis;
  if (absl::SimpleAtoi(ms_since_epoch, &millis)) {
    return std::chrono::system_clock::time_point() + std::chrono::milliseconds(millis);
  }

  return absl::make_optional<std::chrono::system_clock::time_point>();
}

void Message::setStartDeliverTime(std::chrono::system_clock::time_point delivery_timepoint) {
  auto delivery_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(delivery_timepoint.time_since_epoch()).count();
  putSystemProperty(SystemPropKey::STARTDELIVERTIME, std::to_string(delivery_ms));
}

std::string Message::getBody() const { return body_; }

void Message::setBody(absl::string_view body) {
  if (body.empty()) {
    body_.clear();
    return;
  }
  body_ = std::string(body.data(), body.length());
}

std::int32_t Message::getReconsumeTimes() const {
  std::string&& reconsume_time = getSystemProperty(SystemPropKey::RECONSUMETIMES);
  if (reconsume_time.empty()) {
    return 0;
  }

  std::int32_t delivery_attempt = 0;
  if (absl::SimpleAtoi(reconsume_time, &delivery_attempt)) {
    return delivery_attempt;
  }

  return 0;
}

void Message::setReconsumeTimes(std::int32_t reconsume_times) {
  putSystemProperty(SystemPropKey::RECONSUMETIMES, std::to_string(reconsume_times));
}

std::chrono::system_clock::time_point Message::getStoreTimestamp() const { return store_timestamp_; }

void Message::setStoreTimestamp(std::chrono::system_clock::time_point store_timepoint) {
  store_timestamp_ = store_timepoint;
}

std::int64_t Message::getQueueOffset() const { return queue_offset_; }

void Message::setQueueOffset(std::int64_t queue_offset) { queue_offset_ = queue_offset; }

std::string Message::toString() const {
  std::stringstream ss;
  ss << "Message [topic=" << topic_ << ", body=" << body_ << "]";
  return ss.str();
}

std::string Message::toSystemString() const { return absl::StrJoin(system_properties_, ",", absl::PairFormatter("=")); }

std::string Message::toUserString() const { return absl::StrJoin(user_properties_, ",", absl::PairFormatter("=")); }

} // namespace ons
