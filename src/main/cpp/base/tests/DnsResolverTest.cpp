#include "DnsResolver.h"
#include "rocketmq/RocketMQ.h"
#include "gtest/gtest.h"

ROCKETMQ_NAMESPACE_BEGIN

TEST(DnsResolverTest, testResolve) {
  DnsResolver* resolver = dnsResolver();
  resolver->resolve("www.taobao.com", "80");
  auto&& map = resolver->ipDomainMap();
  ASSERT_TRUE(!map.empty());
}

ROCKETMQ_NAMESPACE_END