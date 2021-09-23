#include "OrderConsumerImpl.h"
#include "OrderListenerWrapper.h"

namespace ons {

OrderConsumerImpl::OrderConsumerImpl(const ONSFactoryProperty& factory_property)
    : ONSConsumerAbstract(factory_property) {}

void OrderConsumerImpl::start() { ONSConsumerAbstract::start(); }

void OrderConsumerImpl::shutdown() { ONSConsumerAbstract::shutdown(); }

void OrderConsumerImpl::subscribe(absl::string_view topic, absl::string_view expression) {
  ONSConsumerAbstract::subscribe(topic, expression);
}

void OrderConsumerImpl::registerMessageListener(MessageOrderListener* listener) {
  std::unique_ptr<ROCKETMQ_NAMESPACE::MessageListener> wrapped_listener =
      absl::make_unique<OrderListenerWrapper>(listener);
  ONSConsumerAbstract::registerMessageListener(std::move(wrapped_listener));
}

} // namespace ons