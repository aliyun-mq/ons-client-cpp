#pragma once

#include <string>

#include "absl/strings/string_view.h"

#include "rocketmq/Logger.h"
#include "rocketmq/RocketMQ.h"

#include "spdlog/spdlog.h"

#include "ons/ONSClient.h"

ONS_NAMESPACE_BEGIN

class AccessPoint {
public:
  explicit AccessPoint(absl::string_view access_point);

  explicit operator bool() const;

  const std::string& resourceNamespace() const;

  std::string nameServerAddress() const;

private:
  std::string access_point_;

  static const char* SCHEMA;

  static const char* PREFIX;

  std::string resource_namespace_;
};

ONS_NAMESPACE_END