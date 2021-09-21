#include "ons/ONSEnvironmentVariableFactoryProperty.h"
#include "gtest/gtest.h"
#include <cstdlib>
#include <string>

namespace ons {

class ONSEnvironmentVariableFactoryPropertyTest : public testing::Test {
public:
  void SetUp() override { setenv(ONSFactoryProperty::GroupId, group_id_.c_str(), override_); }

  void TearDown() override {}

protected:
  ONSEnvironmentVariableFactoryProperty factory_property_;
  std::string group_id_{"group_0"};
  int override_{1};
};

TEST_F(ONSEnvironmentVariableFactoryPropertyTest, testEnvironment) {
  std::string&& group_id = factory_property_.getGroupId();
  EXPECT_EQ(group_id_, group_id);
}

} // namespace ons