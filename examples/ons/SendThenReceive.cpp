/*
* Licensed to the Apache Software Foundation (ASF) under one or more
* contributor license agreements.  See the NOTICE file distributed with
* this work for additional information regarding copyright ownership.
* The ASF licenses this file to You under the Apache License, Version 2.0
* (the "License"); you may not use this file except in compliance with
* the License.  You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
 */
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "ons/ONSFactory.h"
#include "rocketmq/Logger.h"
#include "spdlog/spdlog.h"

using namespace ons;

class ExampleMessageOrderListener : public ons::MessageOrderListener {
public:
  OrderAction consume(const Message& message, const ConsumeOrderContext& context) noexcept override {
    SPDLOG_INFO("Consume message[MsgId={}] OK", message.getMsgID());
    std::cout << "Consume Message[MsgId=" << message.getMsgID() << "] OK" << std::endl;
    return OrderAction::Success;
  }
};

int main(int argc, char* argv[]) {
  rocketmq::Logger& logger = rocketmq::getLogger();
  logger.setLevel(rocketmq::Level::Debug);
  logger.setConsoleLevel(rocketmq::Level::Debug);
  logger.init();

  std::cout << "=======Before consuming messages=======" << std::endl;
  ONSFactoryProperty factory_property;
  factory_property.setFactoryProperty(ONSFactoryProperty::GroupId, "GID_sdk_fifo");
  std::string topic = "sdk_fifo";
  std::string tag = "*";

  auto consumer = [&] {
    OrderConsumer* consumer = ONSFactory::getInstance()->createOrderConsumer(factory_property);
    consumer->subscribe(topic, tag);
    auto listener = new ExampleMessageOrderListener();
    consumer->registerMessageListener(listener);
    consumer->start();
    std::this_thread::sleep_for(std::chrono::minutes(3));
    consumer->shutdown();
    delete listener;
  };

  auto producer = [&] {
    auto order_producer = ONSFactory::getInstance()->createOrderProducer(factory_property);
    order_producer->start();

    Message message("sdk_fifo", "TagA", "Sample Body");
    for (int i = 0; i < 256; ++i) {
      SendResultONS send_result = order_producer->send(message, "message-group-0");
      std::cout << "Message sent with message-id=" << send_result.getMessageId() << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds (100));
    }
    order_producer->shutdown();
  };


  std::thread t_consumer(consumer);
  std::thread t_producer(producer);

  t_producer.join();
  t_consumer.join();

  return EXIT_SUCCESS;
}