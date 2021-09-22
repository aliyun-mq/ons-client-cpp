#include "TransactionProducerImpl.h"

#include "absl/strings/ascii.h"

#include "ons/SendResultONS.h"

namespace ons {

// TODO: not yet implemented
TransactionProducerImpl::TransactionProducerImpl(const ONSFactoryProperty& factory_property,
                                                 LocalTransactionChecker* checker)
    : ONSClientAbstract(factory_property), producer_(factory_property.getGroupId()) {
  auto send_msg_timeout = factory_property.getSendMsgTimeout();
  if (send_msg_timeout.count()) {
    producer_.setSendMsgTimeout(send_msg_timeout);
  }

  absl::string_view instanceName = absl::StripAsciiWhitespace(factory_property.getInstanceId());
  if (instanceName.empty()) {
    producer_.setInstanceName(buildInstanceName());
  } else {
    producer_.setInstanceName(std::string(instanceName));
  }
  int send_msg_retry_times = factory_property.getSendMsgRetryTimes();
  if (send_msg_retry_times > 0) {
    producer_.setMaxAttemptTimes(send_msg_retry_times);
  }

  auto credentials_provider = std::make_shared<ROCKETMQ_NAMESPACE::StaticCredentialsProvider>(
      factory_property.getAccessKey(), factory_property.getSecretKey());
  producer_.setCredentialsProvider(credentials_provider);

  if (access_point_) {
    absl::string_view resource_namespace = access_point_.resourceNamespace();
    producer_.setResourceNamespace(std::string(resource_namespace.data(), resource_namespace.length()));
    producer_.setNamesrvAddr(access_point_.nameServerAddress());
  } else if (!factory_property.getNameSrvAddr().empty()) {
    producer_.setNamesrvAddr(factory_property.getNameSrvAddr());
  } else if (!factory_property.getNameSrvDomain().empty()) {
    producer_.setNameServerListDiscoveryEndpoint(factory_property.getNameSrvDomain());
  }
}

void TransactionProducerImpl::start() { producer_.start(); }

void TransactionProducerImpl::shutdown() { producer_.shutdown(); }

SendResultONS TransactionProducerImpl::send(Message& msg, LocalTransactionExecuter* executor) {

  return SendResultONS();
}

} // namespace ons