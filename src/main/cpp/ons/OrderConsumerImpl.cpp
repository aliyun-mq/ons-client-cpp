#include "OrderConsumerImpl.h"

namespace ons {

OrderConsumerImpl::OrderConsumerImpl(const ONSFactoryProperty& factory_property) {}

void OrderConsumerImpl::start() {}

void OrderConsumerImpl::shutdown() {}

void OrderConsumerImpl::subscribe(const char* topic, const char* expression) {}

void OrderConsumerImpl::registerMessageListener(MessageOrderListener* listener) {}

} // namespace ons