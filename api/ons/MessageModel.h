#pragma once

#include <cstdint>

namespace ons {

enum class MessageModel : std::uint8_t {
  CLUSTERING = 0,
  BROADCASTING = 1,
};

}