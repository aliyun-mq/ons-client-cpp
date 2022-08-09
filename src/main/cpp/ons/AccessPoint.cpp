#include "AccessPoint.h"

#include <vector>

#include "absl/strings/str_replace.h"
#include "absl/strings/str_split.h"

ONS_NAMESPACE_BEGIN

const char* AccessPoint::PREFIX = "http://MQ_INST_";

const char* AccessPoint::SCHEMA = "http://";

const char* AccessPoint::RESOURCE_NAMESPACE_PREFIX = "MQ_INST_";

AccessPoint::AccessPoint(absl::string_view access_point) : access_point_(access_point.data(), access_point.length()) {
  std::vector<absl::string_view> segments = absl::StrSplit(absl::StripPrefix(access_point_, SCHEMA), '.');
  resource_namespace_ = std::string(segments[0].data(), segments[0].length());
  SPDLOG_INFO("Resource namespace={}, name-server-address={}", resourceNamespace(), nameServerAddress());
}

AccessPoint::operator bool() const {
  if (!absl::StartsWith(access_point_, PREFIX)) {
    return false;
  }

  return absl::StrContains(access_point_, '.');
}

const std::string& AccessPoint::resourceNamespace() const {
  return resource_namespace_;
}

std::string AccessPoint::nameServerAddress() const {
  // If extensive domain name is supported, use the following comment-out line.
  return absl::AsciiStrToLower(absl::StrReplaceAll(absl::StripPrefix(access_point_, SCHEMA), {{"_", "-"}}));
  // If wildcard sub-domain name is preferred
  // absl::string_view name_server =
  //     absl::StripPrefix(absl::StripPrefix(absl::StripPrefix(access_point_, SCHEMA), resourceNamespace()), ".");
  // return std::string(name_server.data(), name_server.length());
}

ONS_NAMESPACE_END