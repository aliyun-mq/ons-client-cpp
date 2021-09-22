#pragma once

#include <chrono>
#include <map>
#include <string>
#include <vector>

#include "absl/strings/string_view.h"
#include "absl/types/optional.h"

#include "MessageModel.h"
#include "ONSChannel.h"
#include "ONSClient.h"
#include "Trace.h"

namespace ons {

class ONSCLIENT_API ONSFactoryProperty {
public:
  ONSFactoryProperty();

  virtual ~ONSFactoryProperty() = default;

  std::string getLogPath() const;

  ONSFactoryProperty& setSendMsgTimeout(int value);
  ONSFactoryProperty& setSendMsgTimeout(std::chrono::milliseconds timeout);
  std::chrono::milliseconds getSendMsgTimeout() const;

  ONSFactoryProperty& setSendMsgRetryTimes(int value);
  ONSFactoryProperty& setMaxMsgCacheSize(int size);

  ONSFactoryProperty& withTraceFeature(Trace trace_flag);
  ONSFactoryProperty& setOnsTraceSwitch(bool should_trace);

  void setOnsChannel(ONSChannel ons_channel);
  void setFactoryProperty(absl::string_view key, absl::string_view value);
  void setFactoryProperties(const std::map<std::string, std::string>& factory_properties);
  std::map<std::string, std::string> getFactoryProperties() const;
  std::string getProducerId() const;
  std::string getConsumerId() const;
  std::string getGroupId() const;

  std::string getMessageModel() const;
  ONSFactoryProperty& setMessageModel(MessageModel message_model);

  /**
   * @brief When processing First-In-First-Out messages, immediate retry on
   * consuming failure may not be desired. Instead, developers prefer to
   * introduce period of time as cool-down.
   *
   * @param duration Period of time to cool down.
   */
  void setSuspendDuration(std::chrono::milliseconds duration);
  std::chrono::milliseconds getSuspendTimeMillis() const;

  int getSendMsgRetryTimes() const;
  int getConsumeThreadNums() const;
  int getMaxMsgCacheSize() const;
  int getMaxMsgCacheSizeInMiB() const;
  ONSChannel getOnsChannel() const;
  std::string getChannel() const;
  std::string getNameSrvAddr() const;
  std::string getNameSrvDomain() const;
  std::string getAccessKey() const;
  std::string getSecretKey() const;
  std::string getConsumerInstanceName() const;
  bool getOnsTraceSwitch() const;
  std::string getInstanceId() const;

  operator bool();

  /**
   * @brief Path of log files. If unspecified, defaults to ${HOME}/logs/rocketmq
   */
  static const char* LogPath;

  // Deprecated, please use GroupId instead.
  static const char* ProducerId;

  // Deprecated, please use GroupId instead.
  static const char* ConsumerId;

  /**
   * @brief Name of the load-balancing group. Take message consuming for
   * example. If multiple consumer clients work collaborately to fulfill their
   * design goals, they should have the same GroupId and MessageModel set
   * CLUSTERING.
   */
  static const char* GroupId;

  // Deprecated.
  static const char* ONSAddr;

  static const char* AccessKey;
  static const char* SecretKey;

  static const char* MessageModel;
  static const char* BROADCASTING;
  static const char* CLUSTERING;
  static const char* SendMsgTimeoutMillis;
  // Suspend time after failure of consuming order message.
  static const char* SuspendTimeMillis;
  static const char* NAMESRV_ADDR;
  static const char* ConsumeThreadNums;
  static const char* OnsChannel;
  static const char* MaxMsgCacheSize;
  static const char* MaxCachedMessageSizeInMiB;
  static const char* OnsTraceSwitch;
  static const char* SendMsgRetryTimes;
  static const char* ConsumerInstanceName;
  static const char* InstanceId;
  static const char* DEFAULT_CHANNEL;

  static const std::string EMPTY_STRING;

protected:
  void setDefaults();

private:
  std::map<std::string, std::string> property_map_;

  absl::optional<std::string> getProperty(absl::string_view key) const;

  std::string getProperty(absl::string_view key, std::string default_value) const;

  bool validate(const std::string& key, const std::string& value);

  void loadConfigFile();
};
} // namespace ons