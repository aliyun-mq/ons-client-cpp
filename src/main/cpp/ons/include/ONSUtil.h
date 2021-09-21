#pragma once

#include <set>

#include "ons/Message.h"
#include "rocketmq/Logger.h"
#include "rocketmq/MQMessageExt.h"
#include "rocketmq/RocketMQ.h"
#include "spdlog/spdlog.h"

namespace ons {

class ONSUtil {
public:
  ONSUtil();

  static ONSUtil& get();

  Message msgConvert(ROCKETMQ_NAMESPACE::MQMessage& message);

  Message msgConvert(ROCKETMQ_NAMESPACE::MQMessageExt& message_ext);

  ROCKETMQ_NAMESPACE::MQMessage msgConvert(Message& message);

private:
  std::set<std::string> reserved_key_set_;
  std::set<std::string> reserved_key_set_ext_;
};

} // namespace ons