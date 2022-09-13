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
#include "BroadcastTask.h"
#include "ConsumeMessageService.h"
#include "ProcessQueue.h"
#include "spdlog/spdlog.h"

ROCKETMQ_NAMESPACE_BEGIN

BroadcastTask::BroadcastTask(std::weak_ptr<ProcessQueue> process_queue,
                             std::weak_ptr<ConsumeMessageService> consume_message_service)
    : process_queue_(std::move(process_queue)), consume_message_service_(std::move(consume_message_service)) {
}

void BroadcastTask::process() {
  auto service = consume_message_service_.lock();
  if (!service) {
    return;
  }

  auto process_queue = process_queue_.lock();
  if (!process_queue) {
    return;
  }

  bool expected = false;
  if (runnable_.compare_exchange_strong(expected, true, std::memory_order_relaxed)) {

    while (true) {
      auto optional = process_queue->dequeBroadcastMessage();
      if (!optional.has_value()) {
        break;
      }
      service->preHandle(optional.value());

      ConsumeMessageResult result;

      auto listener = service->listener();
      switch (listener->listenerType()) {
        case MessageListenerType::FIFO: {
          result = dynamic_cast<FifoMessageListener*>(listener)->consumeMessage(optional.value());
          break;
        }
        case MessageListenerType::STANDARD: {
          result = dynamic_cast<StandardMessageListener*>(listener)->consumeMessage({optional.value()});
          break;
        }
      }
      service->postHandle(optional.value(), result);
      process_queue->release(optional.value().bodyLength());
    }

    expected = true;
    if (!runnable_.compare_exchange_strong(expected, false, std::memory_order_relaxed)) {
      SPDLOG_WARN("Unexpected runnable state");
    }
  }
}

ROCKETMQ_NAMESPACE_END