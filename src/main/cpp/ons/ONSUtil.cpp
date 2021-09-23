#include "ONSUtil.h"

#include <chrono>

#include "Protocol.h"
#include "rocketmq/RocketMQ.h"

namespace ons {

ONSUtil::ONSUtil() {
  reserved_key_set_ext_.insert(SystemPropKey::TAG);
  reserved_key_set_ext_.insert(SystemPropKey::MSGID);
  reserved_key_set_ext_.insert(SystemPropKey::RECONSUMETIMES);
  reserved_key_set_ext_.insert(SystemPropKey::STARTDELIVERTIME);
}

ONSUtil& ONSUtil::get() {
  static ONSUtil ons_util;
  return ons_util;
}

Message ONSUtil::msgConvert(const ROCKETMQ_NAMESPACE::MQMessage& rocketmq_message) {
  Message message;
  if (!rocketmq_message.getTopic().empty()) {
    message.setTopic(rocketmq_message.getTopic());
  }

  if (!rocketmq_message.getKeys().empty()) {
    for (const auto& key : rocketmq_message.getKeys()) {
      message.attachKey(key);
    }
  }

  if (!rocketmq_message.getTags().empty()) {
    message.setTag(rocketmq_message.getTags());
  }

  if (!rocketmq_message.getBody().empty()) {
    message.setBody(rocketmq_message.getBody());
  }

  std::map<std::string, std::string> properties = rocketmq_message.getProperties();
  if (!properties.empty()) {
    auto it = properties.begin();
    for (; it != properties.end(); ++it) {
      // System
      auto its = reserved_key_set_.find(it->first);
      if (its == reserved_key_set_.end()) {
        auto ite = reserved_key_set_ext_.find(it->first);
        if (ite != reserved_key_set_ext_.end()) {
          message.putSystemProperty(it->first, it->second);
          SPDLOG_DEBUG("msgConvert: msg after putSystemProperties:{}", message.toSystemString());
        }
        // User
        else {
          message.putUserProperty(it->first, it->second);
          SPDLOG_DEBUG("msgConvert: msg after putUserProperties:{}", message.toUserString());
        }
      }
    }
  }
  SPDLOG_DEBUG("msgConvert: after msgConvert, Message:{}, systemString:{}, "
               "userString:{}",
               message.toString().c_str(), message.toSystemString().c_str(), message.toUserString().c_str());
  return message;
}

Message ONSUtil::msgConvert(const ROCKETMQ_NAMESPACE::MQMessageExt& rocketmq_message_ext) {
  Message message;

  if (!rocketmq_message_ext.getTopic().empty()) {
    message.setTopic(rocketmq_message_ext.getTopic());
  }

  if (!rocketmq_message_ext.getKeys().empty()) {
    // message.setKey(msgRMQ.getKeys().c_str());
  }

  if (!rocketmq_message_ext.getTags().empty()) {
    message.setTag(rocketmq_message_ext.getTags());
  }

  if (!rocketmq_message_ext.getBody().empty()) {
    message.setBody(rocketmq_message_ext.getBody());
  }
  message.setMsgID(rocketmq_message_ext.getMsgId());

  message.setReconsumeTimes(rocketmq_message_ext.getDeliveryAttempt());

  auto store_time =
      std::chrono::system_clock::time_point() + std::chrono::milliseconds(rocketmq_message_ext.getStoreTimestamp());
  message.setStoreTimestamp(store_time);

  message.setBornTimestamp(rocketmq_message_ext.bornTimestamp());

  message.setQueueOffset(rocketmq_message_ext.getQueueOffset());

  std::map<std::string, std::string> properties = rocketmq_message_ext.getProperties();
  if (!properties.empty()) {
    auto it = properties.begin();
    for (; it != properties.end(); ++it) {
      // System
      auto its = reserved_key_set_.find(it->first);
      if (its == reserved_key_set_.end()) {
        auto ite = reserved_key_set_ext_.find(it->first);
        if (ite != reserved_key_set_ext_.end()) {
          message.putSystemProperty(it->first, it->second);
        }
        // User
        else {
          message.putUserProperty(it->first, it->second);
        }
      }
    }
  }
  return message;
}

ROCKETMQ_NAMESPACE::MQMessage ONSUtil::msgConvert(const Message& msg) {
  ROCKETMQ_NAMESPACE::MQMessage message;
  if (!msg.getTopic().empty()) {
    message.setTopic(msg.getTopic());
  }

  if (!msg.getKeys().empty()) {
    message.setKeys(msg.getKeys());
  }

  if (!msg.getTag().empty()) {
    message.setTags(msg.getTag());
  }

  absl::optional<std::chrono::system_clock::time_point> delivery_timepoint = msg.getStartDeliverTime();
  if (delivery_timepoint.has_value()) {
    auto delivery_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(delivery_timepoint->time_since_epoch()).count();
    message.setProperty(SystemPropKey::STARTDELIVERTIME, std::to_string(delivery_ms));
  }

  if (!msg.getBody().empty()) {
    message.setBody(msg.getBody());
  }

  std::map<std::string, std::string> properties = msg.getUserProperties();
  if (!properties.empty()) {
    auto it = properties.begin();
    for (; it != properties.end(); ++it) {
      auto its = reserved_key_set_ext_.find(it->first);
      if (its == reserved_key_set_ext_.end()) {
        message.setProperty(it->first, it->second);
      }
    }
  }
  return message;
}

} // namespace ons
