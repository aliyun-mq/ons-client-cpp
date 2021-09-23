#include "ons/ONSFactory.h"
#include "rocketmq/Logger.h"

#include <iostream>
#include <mutex>
#include <thread>

using namespace std;
using namespace ons;

std::mutex console_mtx;

class ExampleMessageListener : public MessageListener {
public:
  Action consume(Message& message, ConsumeContext& context) override {
    std::lock_guard<std::mutex> lk(console_mtx);
    std::cout << "Received a message. Topic: " << message.getTopic() << ", MsgId: " << message.getMsgID() << std::endl;
    return Action::CommitMessage;
  }
};

int main(int argc, char* argv[]) {
  rocketmq::Logger& logger = rocketmq::getLogger();
  logger.setLevel(rocketmq::Level::Debug);
  logger.init();

  std::cout << "=======Before consuming messages=======" << std::endl;
  ONSFactoryProperty factory_property;

  factory_property.setFactoryProperty(ons::ONSFactoryProperty::GroupId, "GID_cpp_sdk_standard");

  PushConsumer* consumer = ONSFactory::getInstance()->createPushConsumer(factory_property);
  const char* topic = "cpp_sdk_standard";
  const char* tag = "*";

  // register your own listener here to handle the messages received.
  auto* messageListener = new ExampleMessageListener();
  consumer->subscribe(topic, tag);
  consumer->registerMessageListener(messageListener);

  // Start this consumer
  consumer->start();

  // Keep main thread running until process finished.
  std::this_thread::sleep_for(std::chrono::milliseconds(60 * 1000));
  consumer->shutdown();
  std::cout << "=======After consuming messages======" << std::endl;
  return 0;
}