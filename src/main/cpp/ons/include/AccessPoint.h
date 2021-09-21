#pragma once

#include "absl/strings/string_view.h"
#include <string>

namespace ons {

class AccessPoint {
public:
  explicit AccessPoint(absl::string_view access_point) : access_point_(access_point.data(), access_point.length()) {}

  operator bool() const;

  absl::string_view resourceNamespace() const;

  std::string nameServerAddress() const;

private:
  std::string access_point_;

  static const char* SCHEMA;

  static const char* RESOURCE_NAMESPACE_PREFIX;

  static const char* PREFIX;
};

} // namespace ons