#include "ons/ONSFactory.h"
#include "rocketmq/Logger.h"
#include <iostream>

using namespace std;
using namespace ons;

int main(int argc, char* argv[]) {
  rocketmq::Logger& logger = rocketmq::getLogger();
  logger.setLevel(rocketmq::Level::Debug);
  logger.init();

  std::cout << "=======Before sending messages=======" << std::endl;
  ONSFactoryProperty factoryInfo;

  /*
    factoryInfo.setFactoryProperty(ONSFactoryProperty::GroupId, "Your-GroupId");
    factoryInfo.setFactoryProperty(ONSFactoryProperty::AccessKey, "Your-Access-Key");
    factoryInfo.setFactoryProperty(ONSFactoryProperty::SecretKey, "Your-Secret-Key");
    factoryInfo.setFactoryProperty(ONSFactoryProperty::NAMESRV_ADDR, "Your-Access-Point-URL");
  */

  Producer* producer = nullptr;
  producer = ONSFactory::getInstance()->createProducer(factoryInfo);
  producer->start();
  Message msg("cpp_sdk_standard", "Your Tag", "Your Key", "This message body.");

  auto start = std::chrono::system_clock::now();
  int count = 32;
  for (int i = 0; i < count; ++i) {
    try {
      SendResultONS sendResult = producer->send(msg);
      std::cout << "Message ID: " << sendResult.getMessageId() << std::endl;
    } catch (ONSClientException& e) {
      std::cout << "ErrorCode: " << e.what() << std::endl;
    }
  }
  auto interval = std::chrono::system_clock::now() - start;
  std::cout << "Send " << count << " messages OK, costs "
            << std::chrono::duration_cast<std::chrono::milliseconds>(interval).count() << "ms" << std::endl;

  // Keep main thread running until process finished.
  producer->shutdown();
  std::cout << "=======After sending messages=======" << std::endl;
  return 0;
}