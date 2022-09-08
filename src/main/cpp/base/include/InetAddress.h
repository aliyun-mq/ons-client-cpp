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

#include "rocketmq/RocketMQ.h"
#include "absl/strings/string_view.h"
#include "re2/re2.h"


ROCKETMQ_NAMESPACE_BEGIN

class InetAddress {
public:
  InetAddress();

  bool isIPv4(absl::string_view address) const;

  bool isIPv6(absl::string_view address) const;

private:
  re2::RE2 ipv4_pattern_;
  re2::RE2 ipv6_pattern_;

  static const char* IPv4Regex;
  static const char* IPv6Regex;
};

const InetAddress& inetAddress();

ROCKETMQ_NAMESPACE_END