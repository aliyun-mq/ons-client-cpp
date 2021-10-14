#include "ons/ONSFactoryProperty.h"
#include "gtest/gtest.h"

ONS_NAMESPACE_BEGIN

class ONSFactoryPropertyTest : public testing::Test {
public:
  ONSFactoryPropertyTest() : factory_property_(false) {}

protected:
  ONSFactoryProperty factory_property_;
  std::string group_id_{"group-0"};
};

TEST_F(ONSFactoryPropertyTest, testGroupId) {
  EXPECT_TRUE(factory_property_.getGroupId().empty());
  EXPECT_TRUE(factory_property_.getConsumerId().empty());

  factory_property_.setFactoryProperty(ONSFactoryProperty::GroupId, group_id_);
  EXPECT_EQ(group_id_, factory_property_.getConsumerId());
  EXPECT_EQ(group_id_, factory_property_.getGroupId());
  EXPECT_EQ(group_id_, factory_property_.getProducerId());
}

ONS_NAMESPACE_END