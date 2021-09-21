#pragma once

#include <cstdint>

namespace ons {

enum class ONSChannel : uint8_t {
  CLOUD = 0,
  ALIYUN = 1,
  ALL = 2,
  LOCAL = 3,
  INNER = 4,
};

}