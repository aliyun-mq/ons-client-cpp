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

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <memory>
#include <set>

#include "ClientManager.h"
#include "FilterExpression.h"
#include "MixAll.h"
#include "ProcessQueue.h"
#include "ReceiveMessageCallback.h"
#include "TopicAssignmentInfo.h"
#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "apache/rocketmq/v1/service.pb.h"
#include "rocketmq/MQMessageExt.h"

ROCKETMQ_NAMESPACE_BEGIN

class PushConsumer;

/**
 * @brief Once messages are fetched(either pulled or popped) from remote server, they are firstly put into cache.
 * Dispatcher thread, after waking up, will submit them into thread-pool. Messages at this phase are called "inflight"
 * state. Once messages are processed by user-passed-in callback, their quota will be released for future incoming
 * messages.
 */
class ProcessQueueImpl : virtual public ProcessQueue, public std::enable_shared_from_this<ProcessQueueImpl> {
public:
  ProcessQueueImpl(MQMessageQueue message_queue, FilterExpression filter_expression,
                   std::weak_ptr<PushConsumer> consumer, std::shared_ptr<ClientManager> client_instance);

  ~ProcessQueueImpl() override;

  void callback(std::shared_ptr<AsyncReceiveMessageCallback> callback) override;

  std::shared_ptr<AsyncReceiveMessageCallback> callback() const override {
    return receive_callback_;
  }

  bool expired() const override;

  bool shouldThrottle() const override;

  const FilterExpression& getFilterExpression() const override;

  std::weak_ptr<PushConsumer> getConsumer() override;

  std::shared_ptr<ClientManager> getClientManager() override;

  void receiveMessage() override;

  const std::string& simpleName() const override {
    return simple_name_;
  }

  std::string topic() const override {
    return message_queue_.getTopic();
  }

  std::uint64_t cachedMessageQuantity() const override;

  std::uint64_t cachedMessageMemory() const override;

  /**
   * Put message fetched from broker into cache.
   *
   * @param messages
   */
  void accountCache(const std::vector<MQMessageExt>& messages) override;

  void syncIdleState() override {
    idle_since_ = std::chrono::steady_clock::now();
  }

  void release(uint64_t body_size) override;

  const MQMessageQueue& messageQueue() const override {
    return message_queue_;
  }

  std::int64_t nextOffset() const override {
    return next_offset_;
  }

  void nextOffset(std::int64_t value) override {
    next_offset_ = value;
  }

  void enqueueBroadcastMessages(std::vector<MQMessageExt> messages) override LOCKS_EXCLUDED(broadcast_messages_mtx_) {
    absl::MutexLock lk(&broadcast_messages_mtx_);
    for (const auto& message : messages) {
      broadcast_messages_.push_back(message);
    }
  }

  absl::optional<MQMessageExt> dequeBroadcastMessage() override LOCKS_EXCLUDED(broadcast_messages_mtx_) {
    absl::MutexLock lk(&broadcast_messages_mtx_);
    if (broadcast_messages_.empty()) {
      return {};
    }

    MQMessageExt message(broadcast_messages_.front());
    broadcast_messages_.pop_front();
    return {message};
  }

  std::shared_ptr<BroadcastTask> broadcastTask() const override {
    return broadcast_task_;
  }

  void broadcastTask(std::shared_ptr<BroadcastTask> task) override {
    broadcast_task_ = std::move(task);
  }

private:
  MQMessageQueue message_queue_;

  /**
   * Expression used to filter message in the server side.
   */
  const FilterExpression filter_expression_;

  std::chrono::milliseconds invisible_time_;

  std::chrono::steady_clock::time_point idle_since_{std::chrono::steady_clock::now()};

  absl::Time create_timestamp_{absl::Now()};

  std::string simple_name_;

  std::weak_ptr<PushConsumer> consumer_;
  std::shared_ptr<ClientManager> client_manager_;

  std::shared_ptr<AsyncReceiveMessageCallback> receive_callback_;

  /**
   * @brief Quantity of the cached messages.
   *
   */
  std::atomic<uint32_t> cached_message_quantity_{0};

  /**
   * @brief Total body memory size of the cached messages.
   *
   */
  std::atomic<uint64_t> cached_message_memory_{0};

  std::int64_t next_offset_{-1};

  std::deque<MQMessageExt> broadcast_messages_ GUARDED_BY(broadcast_messages_mtx_);
  absl::Mutex broadcast_messages_mtx_;

  std::shared_ptr<BroadcastTask> broadcast_task_;

  void popMessage();
  void wrapPopMessageRequest(absl::flat_hash_map<std::string, std::string>& metadata,
                             rmq::ReceiveMessageRequest& request);

  void pullMessage();
  void wrapPullMessageRequest(absl::flat_hash_map<std::string, std::string>& metadata,
                              rmq::PullMessageRequest& request);

  void wrapFilterExpression(rmq::FilterExpression* filter_expression);
};

ROCKETMQ_NAMESPACE_END