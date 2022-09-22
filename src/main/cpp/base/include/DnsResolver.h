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

#include "absl/base/thread_annotations.h"
#include "absl/container/flat_hash_map.h"
#include "absl/synchronization/mutex.h"
#include "absl/types/optional.h"
#include "absl/strings/string_view.h"
#include "rocketmq/RocketMQ.h"
#include <string>

ROCKETMQ_NAMESPACE_BEGIN

class DnsResolver {
public:
  void resolve(const std::string& host, const char* port) LOCKS_EXCLUDED(ip_domain_map_mtx_);

  /**
   * Each element is in the form of domain:port;
   * Elements are joined by ';' separator.
   *
   * @param list
   */
  void resolve(const std::vector<std::string>& list);

  absl::optional<std::string> lookupIP(absl::string_view ip) LOCKS_EXCLUDED(ip_domain_map_mtx_);

  /**
   * Expose for test purpose.
   */
  absl::flat_hash_map<std::string, std::string> ipDomainMap() LOCKS_EXCLUDED(ip_domain_map_mtx_) {
    absl::MutexLock lk(&ip_domain_map_mtx_);
    return ip_domain_map_;
  }

private:
  absl::flat_hash_map<std::string, std::string> ip_domain_map_ GUARDED_BY(ip_domain_map_mtx_);
  absl::Mutex ip_domain_map_mtx_;
};

DnsResolver* dnsResolver();

ROCKETMQ_NAMESPACE_END