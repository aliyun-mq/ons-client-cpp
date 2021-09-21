#include "SendCallbackONSWrapper.h"
#include "rocketmq/RocketMQ.h"

namespace ons {

SendCallbackONSWrapper::SendCallbackONSWrapper(SendCallbackONS* send_callback_ons_ptr)
    : send_callback_ons_ptr_(send_callback_ons_ptr) {}

void SendCallbackONSWrapper::onSuccess(ROCKETMQ_NAMESPACE::SendResult& send_result) {
  if (nullptr == send_callback_ons_ptr_) {
    return;
  }
  SendResultONS send_result_ons;
  send_result_ons.setMessageId(send_result.getMsgId());
  send_callback_ons_ptr_->onSuccess(send_result_ons);
}

void SendCallbackONSWrapper::onException(const ROCKETMQ_NAMESPACE::MQException& e) {
  if (nullptr == send_callback_ons_ptr_) {
    return;
  }
  ONSClientException ons_client_exception(e.what());
  send_callback_ons_ptr_->onException(ons_client_exception);
}

} // namespace ons