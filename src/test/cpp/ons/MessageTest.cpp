#include <cstring>

#include "Message.h"
#include "gtest/gtest.h"

ONS_NAMESPACE_BEGIN

TEST(MessageTest, testCtor) {
  Message message;
  EXPECT_TRUE(message.getMsgBody().empty());
  EXPECT_EQ(0, std::strlen(message.getTopic()));
  EXPECT_EQ(0, std::strlen(message.getTag()));
  EXPECT_TRUE(message.getUserProperties().empty());
  EXPECT_TRUE(message.getSystemProperties().empty());
}

TEST(MessageTest, testCtor2) {
  std::string topic{"TestTopic"};
  std::string tag{"TagA"};
  std::string body{R"(Sample body\0 is right here)"};
  Message message(topic, tag, body);
  EXPECT_STREQ(message.getTopic(), topic.c_str());
  EXPECT_STREQ(message.getTag(), tag.c_str());
  EXPECT_STREQ(message.getBody(), body.c_str());
  EXPECT_TRUE(message.getUserProperties().empty());
  EXPECT_EQ(1, message.getSystemProperties().size());
  EXPECT_STREQ(tag.c_str(), message.getSystemProperties(SystemPropKey::TAG));
}

ONS_NAMESPACE_END