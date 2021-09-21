#pragma once

#include <cassert>

#include "absl/base/thread_annotations.h"
#include "absl/synchronization/mutex.h"
#include "ons/ONSCallback.h"
#include "rocketmq/AsyncCallback.h"
#include "rocketmq/MQClientException.h"
#include "rocketmq/RocketMQ.h"
#include "rocketmq/SendResult.h"

namespace ons {

class ONSSendCallback : public ROCKETMQ_NAMESPACE::SendCallback {

public:
  ONSSendCallback() = default;

  static ONSSendCallback* instance() {
    if (instance_ == nullptr) {
      absl::MutexLock lock(&mutex_);
      if (instance_ == nullptr) {
        instance_ = new ONSSendCallback();
      }
    }

    return instance_;
  }

  ~ONSSendCallback() override = default;

  void onSuccess(ROCKETMQ_NAMESPACE::SendResult& send_result) override {
    SendResultONS resultONS;
    resultONS.setMessageId(send_result.getMsgId());
    assert(callback_ != nullptr);
    callback_->onSuccess(resultONS);
  }

  void onException(const ROCKETMQ_NAMESPACE::MQException& e) override {
    ONSClientException ons_exception(e.what(), -1);
    assert(callback_ != nullptr);
    callback_->onException(ons_exception);
  }

  void SetOnsCallBack(SendCallbackONS* send_callback) { callback_ = send_callback; }

private:
  SendCallbackONS* callback_{nullptr};

  static absl::Mutex mutex_;

  static ONSSendCallback* instance_;
};

} // end of namespace  ons
