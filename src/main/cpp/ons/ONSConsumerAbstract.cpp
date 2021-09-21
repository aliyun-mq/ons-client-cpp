#include "ONSConsumerAbstract.h"
#include "absl/strings/ascii.h"
#include "rocketmq/RocketMQ.h"

namespace ons {

ONSConsumerAbstract::ONSConsumerAbstract(const ONSFactoryProperty& factory_property)
    : ONSClientAbstract(factory_property), consumer_(std::string(factory_property.getConsumerId())) {
  int consume_thread_nums = factory_property.getConsumeThreadNums();
  if (consume_thread_nums > 0) {
    consumer_.setConsumeThreadCount(consume_thread_nums);
  }
  consumer_.setNamesrvAddr(factory_property.getNameSrvAddr());

  absl::string_view instanceName = absl::StripAsciiWhitespace(factory_property.getInstanceId());
  if (instanceName.empty()) {
    consumer_.setInstanceName(buildInstanceName());
  } else {
    consumer_.setInstanceName(std::string(instanceName));
  }

  auto credentials_provider = std::make_shared<ROCKETMQ_NAMESPACE::StaticCredentialsProvider>(
      factory_property.getAccessKey(), factory_property.getSecretKey());

  consumer_.setCredentialsProvider(credentials_provider);

  if (access_point_) {
    consumer_.setResourceNamespace(access_point_.resourceNamespace().data());
    consumer_.setNamesrvAddr(access_point_.nameServerAddress());
  } else if (!factory_property.getNameSrvAddr().empty()) {
    consumer_.setNamesrvAddr(factory_property.getNameSrvAddr());
  } else if (!factory_property.getNameSrvDomain().empty()) {
    consumer_.setNameServerListDiscoveryEndpoint(factory_property.getNameSrvDomain());
  }
}

void ONSConsumerAbstract::start() {
  ONSClientAbstract::start();
  consumer_.start();
}

void ONSConsumerAbstract::shutdown() {
  consumer_.shutdown();
  ONSClientAbstract::shutdown();
}

void ONSConsumerAbstract::subscribe(absl::string_view topic, absl::string_view sub_expression) {
  consumer_.subscribe(std::string(topic.data(), topic.length()),
                      std::string(sub_expression.data(), sub_expression.length()));
}

void ONSConsumerAbstract::registerMessageListener(ROCKETMQ_NAMESPACE::MessageListener* mq_message_listener) {
  consumer_.registerMessageListener(mq_message_listener);
}

} // namespace ons