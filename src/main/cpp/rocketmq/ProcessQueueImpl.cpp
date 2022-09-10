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
#include "ProcessQueueImpl.h"

#include <atomic>
#include <chrono>
#include <memory>
#include <system_error>
#include <utility>

#include "AsyncReceiveMessageCallback.h"
#include "ClientManagerImpl.h"
#include "MetadataConstants.h"
#include "Protocol.h"
#include "PushConsumerImpl.h"
#include "ReceiveMessageResult.h"
#include "Signature.h"
#include "apache/rocketmq/v1/service.pb.h"
#include "rocketmq/MQMessageExt.h"
#include "rocketmq/MessageListener.h"

using namespace std::chrono;

ROCKETMQ_NAMESPACE_BEGIN

ProcessQueueImpl::ProcessQueueImpl(MQMessageQueue message_queue, FilterExpression filter_expression,
                                   std::weak_ptr<PushConsumer> consumer, std::shared_ptr<ClientManager> client_instance)
    : message_queue_(std::move(message_queue)), filter_expression_(std::move(filter_expression)),
      invisible_time_(MixAll::millisecondsOf(MixAll::DEFAULT_INVISIBLE_TIME_)),
      simple_name_(message_queue_.simpleName()), consumer_(std::move(consumer)),
      client_manager_(std::move(client_instance)), cached_message_quantity_(0), cached_message_memory_(0) {
  SPDLOG_DEBUG("Created ProcessQueue={}", simpleName());
}

ProcessQueueImpl::~ProcessQueueImpl() {
  SPDLOG_INFO("ProcessQueue={} should have been re-balanced away, thus, is destructed", simpleName());
}

void ProcessQueueImpl::callback(std::shared_ptr<AsyncReceiveMessageCallback> callback) {
  receive_callback_ = callback;
}

bool ProcessQueueImpl::expired() const {
  auto duration = std::chrono::steady_clock::now() - idle_since_;
  if (duration > MixAll::PROCESS_QUEUE_EXPIRATION_THRESHOLD_) {
    SPDLOG_WARN("ProcessQueue={} is expired. It remains idle for {}ms", simpleName(), MixAll::millisecondsOf(duration));
    return true;
  }
  return false;
}

std::uint64_t ProcessQueueImpl::cachedMessageQuantity() const {
  return cached_message_quantity_.load(std::memory_order_relaxed);
}

std::uint64_t ProcessQueueImpl::cachedMessageMemory() const {
  return cached_message_memory_.load(std::memory_order_relaxed);
}

bool ProcessQueueImpl::shouldThrottle() const {
  auto consumer = consumer_.lock();
  if (!consumer) {
    return false;
  }

  std::size_t quantity = cached_message_quantity_.load(std::memory_order_relaxed);
  uint32_t quantity_threshold = consumer->maxCachedMessageQuantity();
  uint64_t memory_threshold = consumer->maxCachedMessageMemory();
  bool need_throttle = quantity >= quantity_threshold;
  if (need_throttle) {
    SPDLOG_INFO("{}: Number of locally cached messages is {}, which exceeds threshold={}", simple_name_, quantity,
                quantity_threshold);
    return true;
  }

  if (memory_threshold) {
    uint64_t bytes = cached_message_memory_.load(std::memory_order_relaxed);
    need_throttle = bytes >= memory_threshold;
    if (need_throttle) {
      SPDLOG_INFO("{}: Locally cached messages take {} bytes, which exceeds threshold={}", simple_name_, bytes,
                  memory_threshold);
      return true;
    }
  }
  return false;
}

void ProcessQueueImpl::receiveMessage() {
  auto consumer = consumer_.lock();
  if (!consumer) {
    return;
  }
  auto message_model = consumer->messageModel();
  switch (message_model) {
    case MessageModel::CLUSTERING: {
      popMessage();
      break;
    }
    case MessageModel::BROADCASTING: {
      pullMessage();
      break;
    }
  }
}

void ProcessQueueImpl::popMessage() {
  rmq::ReceiveMessageRequest request;
  absl::flat_hash_map<std::string, std::string> metadata;
  auto consumer_client = consumer_.lock();
  if (!consumer_client) {
    return;
  }
  Signature::sign(consumer_client.get(), metadata);
  wrapPopMessageRequest(metadata, request);
  syncIdleState();
  SPDLOG_DEBUG("Try to pop message from {}", message_queue_.simpleName());

  client_manager_->receiveMessage(
      message_queue_.serviceAddress(), metadata, request,
      absl::ToChronoMilliseconds(consumer_client->getLongPollingTimeout() + consumer_client->getIoTimeout()),
      receive_callback_);
}

void ProcessQueueImpl::accountCache(const std::vector<MQMessageExt>& messages) {
  auto consumer = consumer_.lock();
  if (!consumer) {
    return;
  }

  for (const auto& message : messages) {
    cached_message_quantity_.fetch_add(1, std::memory_order_relaxed);
    cached_message_memory_.fetch_add(message.getBody().size(), std::memory_order_relaxed);
  }

  SPDLOG_DEBUG("Cache of process-queue={} has {} messages, body of them taking up {} bytes", simple_name_,
               cached_message_quantity_.load(std::memory_order_relaxed),
               cached_message_memory_.load(std::memory_order_relaxed));
}

void ProcessQueueImpl::release(uint64_t body_size) {
  auto consumer = consumer_.lock();
  if (!consumer) {
    return;
  }

  cached_message_quantity_.fetch_sub(1);
  cached_message_memory_.fetch_sub(body_size);
}

void ProcessQueueImpl::wrapFilterExpression(rmq::FilterExpression* filter_expression) {
  assert(filter_expression);
  auto consumer = consumer_.lock();
  if (!consumer) {
    return;
  }
  auto&& optional = consumer->getFilterExpression(message_queue_.getTopic());
  if (optional.has_value()) {
    auto expression = optional.value();
    switch (expression.type_) {
      case TAG:
        filter_expression->set_type(rmq::FilterType::TAG);
        filter_expression->set_expression(expression.content_);
        break;
      case SQL92:
        filter_expression->set_type(rmq::FilterType::SQL);
        filter_expression->set_expression(expression.content_);
        break;
    }
  } else {
    filter_expression->set_type(rmq::FilterType::TAG);
    filter_expression->set_expression("*");
  }
}

void ProcessQueueImpl::wrapPopMessageRequest(absl::flat_hash_map<std::string, std::string>& metadata,
                                             rmq::ReceiveMessageRequest& request) {
  std::shared_ptr<PushConsumer> consumer = consumer_.lock();
  assert(consumer);
  request.mutable_group()->set_name(consumer->getGroupName());
  request.mutable_group()->set_resource_namespace(consumer->resourceNamespace());
  // request.mutable_message_queue()

  wrapFilterExpression(request.mutable_filter_expression());

  // Batch size
  request.set_batch_size(consumer->receiveBatchSize());

  // Set invisible time
  request.mutable_invisible_duration()->set_seconds(
      std::chrono::duration_cast<std::chrono::seconds>(invisible_time_).count());
  auto fraction = invisible_time_ - std::chrono::duration_cast<std::chrono::seconds>(invisible_time_);
  int32_t nano_seconds = static_cast<int32_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(fraction).count());
  request.mutable_invisible_duration()->set_nanos(nano_seconds);
}

void ProcessQueueImpl::pullMessage() {
  auto consumer_client = consumer_.lock();
  if (!consumer_client) {
    return;
  }

  rmq::PullMessageRequest request;
  absl::flat_hash_map<std::string, std::string> metadata;

  Signature::sign(consumer_client.get(), metadata);
  wrapPullMessageRequest(metadata, request);
  syncIdleState();
  SPDLOG_DEBUG("Try to pull message from {}", message_queue_.simpleName());

  std::weak_ptr<ProcessQueue> ptr(shared_from_this());
  auto callback = [ptr](const std::error_code& ec, const ReceiveMessageResult& result) {
    auto process_queue = ptr.lock();
    if (!process_queue) {
      return;
    }
    process_queue->nextOffset(result.next_offset);
    process_queue->callback()->onCompletion(ec, result);
  };

  client_manager_->pullMessage(
      message_queue_.serviceAddress(), metadata, request,
      absl::ToChronoMilliseconds(consumer_client->getLongPollingTimeout() + consumer_client->getIoTimeout()), callback);
}

void ProcessQueueImpl::wrapPullMessageRequest(absl::flat_hash_map<std::string, std::string>& metadata,
                                              rmq::PullMessageRequest& request) {
  std::shared_ptr<PushConsumer> consumer = consumer_.lock();
  if (!consumer) {
    return;
  }

  // Set group
  request.mutable_group()->set_name(consumer->getGroupName());
  request.mutable_group()->set_resource_namespace(consumer->resourceNamespace());

  // Set partition
  request.mutable_partition()->set_id(message_queue_.getQueueId());
  request.mutable_partition()->mutable_topic()->set_name(message_queue_.getTopic());
  request.mutable_partition()->mutable_topic()->set_resource_namespace(consumer->resourceNamespace());
  request.mutable_partition()->mutable_broker()->set_name(message_queue_.getBrokerName());

  // Set offset
  request.set_offset(nextOffset());

  // Set batch_size
  request.set_batch_size(consumer->receiveBatchSize());

  // Set fixed await_time
  request.mutable_await_time()->set_seconds(15);

  // Set filter_expression
  wrapFilterExpression(request.mutable_filter_expression());

  // Set client_id
  request.set_client_id(consumer->clientId());
}

std::weak_ptr<PushConsumer> ProcessQueueImpl::getConsumer() {
  return consumer_;
}

std::shared_ptr<ClientManager> ProcessQueueImpl::getClientManager() {
  return client_manager_;
}

const FilterExpression& ProcessQueueImpl::getFilterExpression() const {
  return filter_expression_;
}

ROCKETMQ_NAMESPACE_END