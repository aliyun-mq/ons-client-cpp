#pragma once

#include "ons/ONSCallback.h"
#include "rocketmq/AsyncCallback.h"
#include "rocketmq/RocketMQ.h"

namespace ons {
class SendCallbackONSWrapper : public ROCKETMQ_NAMESPACE::SendCallback {
public:
  explicit SendCallbackONSWrapper(SendCallbackONS* callback);

  void onSuccess(ROCKETMQ_NAMESPACE::SendResult& send_result) override;

  void onException(const ROCKETMQ_NAMESPACE::MQException& e) override;

private:
  SendCallbackONS* send_callback_ons_ptr_{};
};

} // namespace ons