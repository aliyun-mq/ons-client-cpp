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
#include <cstdlib>
#include <iostream>

#include "ons/ONSFactory.h"
#include "ons/ONSFactoryProperty.h"
#include "rocketmq/Logger.h"

using namespace ons;

int main(int argc, char* argv[]) {
  rocketmq::Logger& logger = rocketmq::getLogger();
  logger.setLevel(rocketmq::Level::Debug);
  logger.setConsoleLevel(rocketmq::Level::Info);
  logger.init();

  ONSFactoryProperty factory_property;

  auto order_producer = ONSFactory::getInstance()->createOrderProducer(factory_property);
  order_producer->start();

  Message message("sdk_fifo", "TagA", "Sample Body");
  for (int i = 0; i < 16; ++i) {
    SendResultONS send_result = order_producer->send(message, "message-group-0");
    std::cout << "Message sent with message-id=" << send_result.getMessageId() << std::endl;
  }


  order_producer->shutdown();

  return EXIT_SUCCESS;
}