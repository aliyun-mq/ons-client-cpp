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
#include "InetAddress.h"

ROCKETMQ_NAMESPACE_BEGIN

const char* InetAddress::IPv4Regex =
    "(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])";

const char* InetAddress::IPv6Regex = "((([0-9a-fA-F]){1,4})\\:){7}([0-9a-fA-F]){1,4}";

const InetAddress& inetAddress() {
  static InetAddress address;
  return address;
}

InetAddress::InetAddress() : ipv4_pattern_(IPv4Regex), ipv6_pattern_(IPv6Regex) {
}

bool InetAddress::isIPv4(absl::string_view address) const {
  return re2::RE2::FullMatch(re2::StringPiece(address.data(), address.length()), ipv4_pattern_);
}

bool InetAddress::isIPv6(absl::string_view address) const {
  return re2::RE2::FullMatch(re2::StringPiece(address.data(), address.length()), ipv6_pattern_);
}

ROCKETMQ_NAMESPACE_END