#pragma once

#include "AccessPoint.h"
#include "ons/ONSFactory.h"

namespace ons {

class ONSClientAbstract {
public:
  explicit ONSClientAbstract(const ONSFactoryProperty& factory_property);

  virtual ~ONSClientAbstract() = default;

  virtual void start();

  virtual void shutdown();

protected:
  std::string buildInstanceName();

  ONSFactoryProperty factory_property_;

  AccessPoint access_point_;
};

} // namespace ons
