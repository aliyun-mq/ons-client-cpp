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

#include <cstdint>
#include <memory>

#include "ConsumeMessageType.h"
#include "FilterExpression.h"
#include "rocketmq/MQMessageExt.h"

ROCKETMQ_NAMESPACE_BEGIN

class PushConsumer;

class ClientManager;

class AsyncReceiveMessageCallback;

class ProcessQueue {
public:
  virtual ~ProcessQueue() = default;

  virtual bool expired() const = 0;

  virtual void callback(std::shared_ptr<AsyncReceiveMessageCallback> callback) = 0;

  virtual void receiveMessage() = 0;

  virtual std::string topic() const = 0;

  virtual std::weak_ptr<PushConsumer> getConsumer() = 0;

  virtual const std::string& simpleName() const = 0;

  virtual void release(uint64_t body_size) = 0;

  virtual void accountCache(const std::vector<MQMessageExt>& messages) = 0;

  virtual std::uint64_t cachedMessageQuantity() const = 0;

  virtual std::uint64_t cachedMessageMemory() const = 0;

  virtual bool shouldThrottle() const = 0;

  virtual std::shared_ptr<ClientManager> getClientManager() = 0;

  virtual void syncIdleState() = 0;

  virtual const FilterExpression& getFilterExpression() const = 0;

  virtual const MQMessageQueue& messageQueue() const = 0;

  virtual std::int64_t nextOffset() const = 0;
  virtual void nextOffset(std::int64_t value) = 0;
};

using ProcessQueueWeakPtr = std::weak_ptr<ProcessQueue>;
using ProcessQueueSharedPtr = std::shared_ptr<ProcessQueue>;

ROCKETMQ_NAMESPACE_END