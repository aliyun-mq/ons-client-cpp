#pragma once

#include <cstdint>

namespace ons {

enum class TransactionStatus : std::uint8_t {
  CommitTransaction = 0,
  RollbackTransaction = 1,
  Unknow = 2,
};

}