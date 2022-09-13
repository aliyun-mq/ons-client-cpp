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
#include "ons/ONSFactory.h"
#include "rocketmq/Logger.h"

#include <chrono>
#include <iostream>
#include <system_error>

using namespace std;
using namespace ons;

int main(int argc, char* argv[]) {
  rocketmq::Logger& logger = rocketmq::getLogger();
  logger.setLevel(rocketmq::Level::Warn);
  logger.setConsoleLevel(rocketmq::Level::Warn);
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
  Message msg("sdk_standard", "Your Tag", "Your Key", "This message body.");

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  //                             Send with exception                                                //
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  auto start = std::chrono::system_clock::now();
  int count = 3200;
  for (int i = 0; i < count; ++i) {
    try {
      SendResultONS sendResult = producer->send(msg);
      std::cout << "Message ID: " << sendResult.getMessageId() << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } catch (ONSClientException& e) {
      std::cout << "ErrorCode: " << e.what() << std::endl;
    }
  }
  auto interval = std::chrono::system_clock::now() - start;
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(interval).count();
  std::cout << "Send " << count << " messages OK, costs " << ms << "ms. AVG QPS: " << count * 1000 * 1.0 / ms
            << " AVG Latency: " << ms * 1.0 / count << "ms" << std::endl;

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  //                            Send without exception support                                      //
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  start = std::chrono::system_clock::now();
  for (int i = 0; i < count; i++) {
    std::error_code ec;
    SendResultONS send_result = producer->send(msg, ec);
    if (ec) {
      std::cerr << "Failed to send message. Cause: " << ec.message() << std::endl;
      continue;
    }
    std::cout << "MessageID: " << send_result.getMessageId() << std::endl;
  }
  interval = std::chrono::system_clock::now() - start;
  ms = std::chrono::duration_cast<std::chrono::milliseconds>(interval).count();
  std::cout << "Send " << count << " messages OK, costs " << ms << "ms. AVG QPS: " << count * 1000 * 1.0 / ms
            << " AVG Latency: " << ms * 1.0 / count << "ms" << std::endl;

  // Keep main thread running until process finished.
  producer->shutdown();
  std::cout << "=======After sending messages=======" << std::endl;
  return 0;
}