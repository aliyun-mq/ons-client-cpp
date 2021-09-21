#pragma once

// order consuming result
enum OrderAction {
  // consume success, application could continue to consume next message
  Success,
  // consume fail, suspends the current queue
  Suspend,
};