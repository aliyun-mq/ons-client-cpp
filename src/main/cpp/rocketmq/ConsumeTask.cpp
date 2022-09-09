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

#include "ConsumeTask.h"

#include "MessageAccessor.h"
#include "PushConsumerImpl.h"
#include "rocketmq/ErrorCode.h"
#include "rocketmq/Logger.h"
#include "rocketmq/MQMessageExt.h"
#include "rocketmq/MessageListener.h"
#include "spdlog/spdlog.h"

ROCKETMQ_NAMESPACE_BEGIN

ConsumeTask::ConsumeTask(ConsumeMessageServiceWeakPtr service, std::weak_ptr<ProcessQueue> process_queue,
                         MQMessageExt message)
    : service_(service), process_queue_(std::move(process_queue)) {
  messages_.emplace_back(message);
}

ConsumeTask::ConsumeTask(ConsumeMessageServiceWeakPtr service, std::weak_ptr<ProcessQueue> process_queue,
                         std::vector<MQMessageExt> messages)
    : service_(service), process_queue_(std::move(process_queue)), messages_(std::move(messages)) {
  fifo_ = messages_.size() > 1;
}

void ConsumeTask::pop() {
  assert(!messages_.empty());
  auto process_queue = process_queue_.lock();
  if (!process_queue) {
    return;
  }

  process_queue->release(messages_[0].getBody().size());

  messages_.erase(messages_.begin());
}

void ConsumeTask::submit() {
  auto svc = service_.lock();
  if (!svc) {
    return;
  }
  svc->submit(shared_from_this());
}

void ConsumeTask::schedule() {
  auto svc = service_.lock();
  if (!svc) {
    return;
  }

  svc->schedule(shared_from_this(), std::chrono::seconds(1));
}

void ConsumeTask::onAck(std::shared_ptr<ConsumeTask> task, const std::error_code& ec) {
  // Treat both success and invalid-receipt-handle as completion
  if (!ec) {
    task->pop();
    task->next_step_ = NextStep::Consume;
    task->submit();
    return;
  }

  // Try to ack again later
  SPDLOG_WARN("Failed to ack message[message-id={}]. Cause: {}. Action: retry after 1s.", task->messages_[0].getMsgId(),
              ec.message());
  task->next_step_ = NextStep::Ack;
  task->schedule();
}

void ConsumeTask::onNack(std::shared_ptr<ConsumeTask> task, const std::error_code& ec) {
  assert(!task->fifo_);
  assert(!task->messages_.empty());

  // Treat both success and invalid-receipt-handle as completion
  if (!ec) {
    task->pop();
    task->next_step_ = NextStep::Consume;
    task->submit();
    return;
  }

  SPDLOG_WARN("Failed to nack message[message-id={}]. Cause: {}. Action: retry after 1s.",
              task->messages_[0].getMsgId(), ec.message());
  task->next_step_ = NextStep::Nack;
  task->schedule();
}

void ConsumeTask::onForward(std::shared_ptr<ConsumeTask> task, bool successful) {
  assert(task->fifo_);
  assert(!task->messages_.empty());

  // Treat both success and invalid-receipt-handle as completion
  if (!successful) {
    SPDLOG_DEBUG("Message[message-id={}] is forwarded to DLQ", task->messages_[0].getMsgId());
    task->pop();
    task->next_step_ = NextStep::Consume;
    task->submit();
    return;
  }

  SPDLOG_DEBUG("Failed to forward Message[message-id={}] to DLQ. Action: retry after 1s.",
               task->messages_[0].getMsgId());
  task->next_step_ = NextStep::Forward;
  task->schedule();
}

void ConsumeTask::process() {
  auto svc = service_.lock();
  if (!svc) {
    SPDLOG_DEBUG("ConsumeMessageService has destructed");
    return;
  }

  if (messages_.empty()) {
    SPDLOG_DEBUG("No more messages to process");
    return;
  }

  std::shared_ptr<PushConsumerImpl> consumer = svc->consumer().lock();

  auto self = shared_from_this();

  switch (next_step_) {
    case NextStep::Consume: {
      const auto& listener = svc->listener();
      auto it = messages_.begin();
      SPDLOG_DEBUG("Start to process message[message-id={}]", it->getMsgId());
      svc->preHandle(*it);

      std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

      // Invoke user-defined-callback
      ConsumeMessageResult result;
      if (MessageListenerType::FIFO == listener->listenerType()) {
        auto fifo_message_listener = dynamic_cast<FifoMessageListener*>(listener);
        result = fifo_message_listener->consumeMessage(*it);
      } else {
        auto standard_message_listener = dynamic_cast<StandardMessageListener*>(listener);
        result = standard_message_listener->consumeMessage({*it});
      }
      svc->postHandle(*it, result);

      switch (result) {
        case ConsumeMessageResult::SUCCESS: {
          auto callback = std::bind(&ConsumeTask::onAck, self, std::placeholders::_1);
          svc->ack(*it, callback);
          break;
        }
        case ConsumeMessageResult::FAILURE: {
          if (fifo_) {
            next_step_ = NextStep::Consume;
            // Increase delivery attempts.
            MessageAccessor::setDeliveryAttempt(*it, it->getDeliveryAttempt() + 1);
            schedule();
          } else {
            // For standard way of processing, Nack to server.
            auto callback = std::bind(&ConsumeTask::onNack, self, std::placeholders::_1);
            svc->nack(*it, callback);
          }
          break;
        }
      }
      break;
    }

    case NextStep::Ack: {
      assert(!messages_.empty());
      auto callback = std::bind(&ConsumeTask::onAck, self, std::placeholders::_1);
      svc->ack(messages_[0], callback);
      break;
    }

    case NextStep::Nack: {
      auto callback = std::bind(&ConsumeTask::onNack, self, std::placeholders::_1);
      svc->nack(messages_[0], callback);
      break;
    }

    case NextStep::Forward: {
      assert(!messages_.empty());
      auto callback = std::bind(&ConsumeTask::onForward, self, std::placeholders::_1);
      svc->forward(messages_[0], callback);
      break;
    }
  }
}

ROCKETMQ_NAMESPACE_END