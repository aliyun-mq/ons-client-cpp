#pragma once

namespace ons {

enum TransactionStatus {
  CommitTransaction = 0,
  RollbackTransaction = 1,
  Unknow = 2,
};
}