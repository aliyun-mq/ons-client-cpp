#pragma once

#include <cstdint>

#include "ONSClient.h"

// consuming result
enum class Action : std::int8_t {
  // consume success, application could continue to consume next message
  CommitMessage,

  // consume fail, server will deliver this message later, application could
  // continue to consume next message
  ReconsumeLater
};
