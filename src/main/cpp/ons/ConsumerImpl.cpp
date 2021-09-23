#include "ConsumerImpl.h"

#include "absl/memory/memory.h"

#include "MessageListenerWrapper.h"

namespace ons {

ConsumerImpl::ConsumerImpl(const ONSFactoryProperty& ons_factory_property)
    : ONSConsumerAbstract(ons_factory_property) {}

void ConsumerImpl::start() { ONSConsumerAbstract::start(); }

void ConsumerImpl::shutdown() { ONSConsumerAbstract::shutdown(); }

void ConsumerImpl::subscribe(absl::string_view topic, absl::string_view sub_expression) {
  if (topic.empty()) {
    THROW_ONS_EXCEPTION(ONSClientException, "Subscribed topic is null", OTHER_ERROR);
  }

  if (sub_expression.empty()) {
    THROW_ONS_EXCEPTION(ONSClientException, "SubExpression topic is null", OTHER_ERROR);
  }

  ONSConsumerAbstract::subscribe(topic, sub_expression);
}

void ConsumerImpl::registerMessageListener(MessageListener* listener) {
  if (nullptr == listener) {
    THROW_ONS_EXCEPTION(ONSClientException, "MessageListener may not be nullptr.", CONSUME_MESSAGE_LISTENER_IS_NULL);
  }

  auto message_listener = absl::make_unique<MessageListenerWrapper>(listener);
  ONSConsumerAbstract::registerMessageListener(std::move(message_listener));
}

} // namespace ons