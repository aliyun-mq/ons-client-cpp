#pragma once

#include "ONSClientException.h"
#include "SendResultONS.h"

namespace ons {
class SendCallbackONS {
public:
  virtual ~SendCallbackONS() = default;
  virtual void onSuccess(SendResultONS& sendResult) = 0;
  virtual void onException(ONSClientException& e) = 0;
};

} // namespace ons
