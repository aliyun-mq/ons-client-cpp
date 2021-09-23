#include "ONSUtil.h"
#include "rocketmq/MQMessageExt.h"

#include "gtest/gtest.h"
#include <vector>

namespace ons {

namespace rocketmq_message_to_ons_message {
  TEST(ONSUtilTest, testConvert_Key) {
    ROCKETMQ_NAMESPACE::MQMessageExt message;
    std::vector<std::string> keys{"k1", "k2"};
    message.setKeys(keys);

    ons::Message msg = ONSUtil::get().msgConvert(message);
    ASSERT_EQ(keys, msg.getKeys());
  }

  TEST(ONSUtilTest, testConvert_Tag) {
    ROCKETMQ_NAMESPACE::MQMessageExt message;
    std::string tag{"TagA"};
    message.setTags(tag);

    ons::Message msg = ONSUtil::get().msgConvert(message);
    ASSERT_EQ(tag, msg.getTag());
  }

  TEST(ONSUtilTest, testConvert_Body) {
    ROCKETMQ_NAMESPACE::MQMessageExt message;
    std::string body{"Body content"};
    message.setBody(body);

    ons::Message msg = ONSUtil::get().msgConvert(message);
    ASSERT_EQ(body, msg.getBody());
  }

  TEST(ONSUtilTest, testConvert_Topic) {
    ROCKETMQ_NAMESPACE::MQMessageExt message;
    std::string topic{"Topic-Example"};
    message.setTopic(topic);

    ons::Message msg = ONSUtil::get().msgConvert(message);
    ASSERT_EQ(topic, msg.getTopic());
  }

  TEST(ONSUtilTest, testConvert_UserProperties) {
    ROCKETMQ_NAMESPACE::MQMessageExt message;

    std::map<std::string, std::string> maps = {{"k1", "v1"}, {"k2", "v2"}};
    message.setProperties(maps);

    ons::Message msg = ONSUtil::get().msgConvert(message);
    ASSERT_EQ(maps, msg.getUserProperties());
  }
} // namespace rocketmq_message_to_ons_message

namespace ons_message_to_rocketmq_message {

  TEST(ONSUtilTest, testConvert_Key) {
    ons::Message message;
    std::vector<std::string> keys{"k1", "k2"};
    for (const auto& key : keys) {
      message.attachKey(key);
    }

    ROCKETMQ_NAMESPACE::MQMessage msg = ONSUtil::get().msgConvert(message);
    ASSERT_EQ(keys, msg.getKeys());
  }

  TEST(ONSUtilTest, testConvert_Tag) {
    ons::Message message;
    std::string tag{"TagA"};
    message.setTag(tag);

    ROCKETMQ_NAMESPACE::MQMessage msg = ONSUtil::get().msgConvert(message);
    ASSERT_EQ(tag, msg.getTags());
  }

  TEST(ONSUtilTest, testConvert_Body) {
    ons::Message message;
    std::string body{"Body content"};
    message.setBody(body);

    auto msg = ONSUtil::get().msgConvert(message);
    ASSERT_EQ(body, msg.getBody());
  }

  TEST(ONSUtilTest, testConvert_Topic) {
    ons::Message message;
    std::string topic{"Topic-Example"};
    message.setTopic(topic);

    auto msg = ONSUtil::get().msgConvert(message);
    ASSERT_EQ(topic, msg.getTopic());
  }

  TEST(ONSUtilTest, testConvert_UserProperties) {
    ons::Message message;

    std::map<std::string, std::string> maps = {{"k1", "v1"}, {"k2", "v2"}};
    for (const auto& entry : maps) {
      message.putUserProperty(entry.first, entry.second);
    }
    auto msg = ONSUtil::get().msgConvert(message);
    ASSERT_EQ(maps, msg.getProperties());
  }
} // namespace ons_message_to_rocketmq_message

} // namespace ons