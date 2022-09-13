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
#pragma once

#include <memory>
#include <atomic>

#include "rocketmq/RocketMQ.h"

ROCKETMQ_NAMESPACE_BEGIN

class ProcessQueue;
class ConsumeMessageService;

class BroadcastTask : public std::enable_shared_from_this<BroadcastTask> {
public:
  BroadcastTask(std::weak_ptr<ProcessQueue> process_queue, std::weak_ptr<ConsumeMessageService> consume_message_service);

  void process();

  bool runnable() const {
    return runnable_.load(std::memory_order_relaxed);
  }

private:
  std::weak_ptr<ProcessQueue> process_queue_;
  std::weak_ptr<ConsumeMessageService> consume_message_service_;
  std::atomic<bool> runnable_{false};
};

ROCKETMQ_NAMESPACE_END