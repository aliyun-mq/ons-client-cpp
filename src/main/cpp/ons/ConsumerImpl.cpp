#include "ConsumerImpl.h"
#include "MessageListenerWrapper.h"

namespace ons {

ConsumerImpl::ConsumerImpl(const ONSFactoryProperty& ons_factory_property)
    : ONSConsumerAbstract(ons_factory_property) {}

void ConsumerImpl::start() { ONSConsumerAbstract::start(); }

void ConsumerImpl::shutdown() { ONSConsumerAbstract::shutdown(); }

void ConsumerImpl::subscribe(absl::string_view topic, absl::string_view sub_expression, MessageListener* listener) {
  if (nullptr == topic) {
    THROW_ONS_EXCEPTION(ONSClientException, "Subscribed topic is null", OTHER_ERROR);
  }
  if (nullptr == sub_expression) {
    THROW_ONS_EXCEPTION(ONSClientException, "SubExpression topic is null", OTHER_ERROR);
  }
  if (nullptr == listener) {
    THROW_ONS_EXCEPTION(ONSClientException, "MessageListener is not implemented.", SEND_CALLBACK_IS_EMPTY);
  }

  ONSConsumerAbstract::subscribe(topic, sub_expression);

  std::shared_ptr<MessageListenerWrapper> message_listener_wrapper_shared_ptr;
  {
    absl::MutexLock lock(&subscribe_table_mtx_);
    message_listener_wrapper_shared_ptr = std::make_shared<MessageListenerWrapper>(listener);
    subscribe_table_[std::string(topic)] = message_listener_wrapper_shared_ptr;
  }
  ONSConsumerAbstract::registerMessageListener(message_listener_wrapper_shared_ptr.get());
}

} // namespace ons