#pragma once

#include "ONSClientAbstract.h"
#include "SendCallbackONSWrapper.h"
#include "absl/container/flat_hash_map.h"
#include "ons/ONSFactory.h"
#include "ons/Producer.h"
#include "rocketmq/DefaultMQProducer.h"
#include "rocketmq/RocketMQ.h"

namespace ons {
class ProducerImpl : public Producer, public ONSClientAbstract {
public:
  explicit ProducerImpl(const ONSFactoryProperty& factory_property);

  ~ProducerImpl() override = default;

  void start() override;

  void shutdown() override;

  SendResultONS send(Message& message) override;

  SendResultONS send(Message& message, const MessageQueueONS& message_queue) override;

  void sendAsync(Message& message, SendCallbackONS* callback) override;

  void sendOneway(Message& message) override;

private:
  static ROCKETMQ_NAMESPACE::MQMessageQueue messageQueueConvert(const MessageQueueONS& message_queue_ons);

  ROCKETMQ_NAMESPACE::DefaultMQProducer producer_;

  absl::flat_hash_map<SendCallbackONS*, std::shared_ptr<SendCallbackONSWrapper>> callbacks_ GUARDED_BY(callbacks_mtx_);

  absl::Mutex callbacks_mtx_; // protects clients_
};

} // namespace ons