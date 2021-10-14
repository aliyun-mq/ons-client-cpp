#include <string>

#include "AccessPoint.h"
#include "gtest/gtest.h"

ONS_NAMESPACE_BEGIN

class AccessPointTest : public testing::Test {
public:
  AccessPointTest() : access_point_(endpoint_) {
  }

protected:
  std::string endpoint_{"http://MQ_INST_1322518017229982_BAA4lS3a.cn-shanghai.mq-internal.aliyuncs.com:8080"};
  AccessPoint access_point_;

  std::string bad_endpoint_{"http://MQi_INST_1322518017229982_BAA4lS3a.cn-shanghai.mq-internal.aliyuncs.com:8080"};
};

TEST_F(AccessPointTest, testBool) {
  ASSERT_TRUE(access_point_);
  ASSERT_FALSE(AccessPoint(bad_endpoint_));
}

TEST_F(AccessPointTest, testResourceNamespace) {
  const char* expected = "MQ_INST_1322518017229982_BAA4lS3a";
  ASSERT_EQ(expected, access_point_.resourceNamespace());
}

TEST_F(AccessPointTest, testNameServerAddress) {
  const char* expected = "cn-shanghai.mq-internal.aliyuncs.com:8080";
  ASSERT_EQ(expected, access_point_.nameServerAddress());
}

ONS_NAMESPACE_END