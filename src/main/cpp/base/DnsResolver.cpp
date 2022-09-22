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
#include "DnsResolver.h"

#include <iostream>

#include "InetAddress.h"
#include "asio.hpp"
#include "asio/error_code.hpp"
#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"
#include "spdlog/spdlog.h"

ROCKETMQ_NAMESPACE_BEGIN

DnsResolver* dnsResolver() {
  static DnsResolver resolver;
  return &resolver;
}

/**
 * Use asio resolver to make the following implementation portable.
 */
void DnsResolver::resolve(const std::string& host, const char* port) {
  asio::io_context context;
  asio::error_code ec;
  asio::ip::tcp::resolver resolver(context);
  auto result_types = resolver.resolve(host, port, ec);
  for (const asio::ip::tcp::endpoint endpoint : result_types) {
    std::string ip = endpoint.address().to_string();
    {
      absl::MutexLock lk(&ip_domain_map_mtx_);
      ip_domain_map_.insert({ip, host});
    }
    SPDLOG_INFO("DNS resolver cached {} --> {}", ip, host);
  }
}

void DnsResolver::resolve(const std::vector<std::string>& list) {
  for (auto&& segment : list) {
    auto pos = segment.find_last_of(':');
    if (pos != std::string::npos) {
      std::string host = segment.substr(0, pos);
      std::string port = segment.substr(pos + 1);
      if (!inetAddress().isIPv4(host) && !inetAddress().isIPv6(host)) {
        resolve(host, port.data());
      }
    }
  }
}

absl::optional<std::string> DnsResolver::lookupIP(absl::string_view ip) {
  absl::MutexLock lk(&ip_domain_map_mtx_);
  if (ip_domain_map_.contains(ip)) {
    return {ip_domain_map_[ip]};
  }
  return {};
}

ROCKETMQ_NAMESPACE_END