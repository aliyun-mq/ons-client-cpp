#include "ons/ONSEnvironmentVariableFactoryProperty.h"
#include "gtest/gtest.h"
#include <cstdlib>
#include <string>

namespace ons {

class ONSEnvironmentVariableFactoryPropertyTest : public testing::Test {
public:
  void SetUp() override {
    setenv(ONSFactoryProperty::GroupId, group_id_.c_str(), override_);
    setenv(ONSFactoryProperty::AccessKey, access_key_.c_str(), override_);
    setenv(ONSFactoryProperty::SecretKey, secret_key_.c_str(), override_);
    setenv(ONSFactoryProperty::NAMESRV_ADDR, name_server_addr_.c_str(), override_);
  }

  void TearDown() override {}

protected:
  std::string group_id_{"group_0"};
  std::string access_key_{"ak"};
  std::string secret_key_{"sk"};
  std::string name_server_addr_{"ns"};
  int override_{1};
};

TEST_F(ONSEnvironmentVariableFactoryPropertyTest, testEnvironment) {
  ONSEnvironmentVariableFactoryProperty factory_property_;
  std::string&& group_id = factory_property_.getGroupId();
  EXPECT_EQ(group_id_, group_id);
  EXPECT_EQ(access_key_, factory_property_.getAccessKey());
  EXPECT_EQ(secret_key_, factory_property_.getSecretKey());
  EXPECT_EQ(name_server_addr_, factory_property_.getNameSrvAddr());
}

} // namespace ons