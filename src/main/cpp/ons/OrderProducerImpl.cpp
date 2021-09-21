#include "OrderProducerImpl.h"

#include "ONSUtil.h"
#include "rocketmq/MQMessage.h"
#include "rocketmq/RocketMQ.h"

namespace ons {

OrderProducerImpl::OrderProducerImpl(const ONSFactoryProperty& factory_property)
    : ONSClientAbstract(factory_property), producer_(factory_property.getGroupId()) {}

void OrderProducerImpl::start() { producer_.start(); }

void OrderProducerImpl::shutdown() { producer_.shutdown(); }

SendResultONS OrderProducerImpl::send(Message& msg, std::string message_group) {
  ROCKETMQ_NAMESPACE::MQMessage message = ons::ONSUtil::get().msgConvert(msg);
  ROCKETMQ_NAMESPACE::SendResult send_result = producer_.send(message, &selector_, &message_group);
  SendResultONS ons_send_result = SendResultONS();
  ons_send_result.setMessageId(send_result.getMsgId());
  return ons_send_result;
}

} // namespace ons