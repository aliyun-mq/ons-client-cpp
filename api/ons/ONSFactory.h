#pragma once

#include "ONSFactoryAPI.h"

namespace ons {

class ONSCLIENT_API ONSFactory {
public:
  ONSFactory() = delete;

  virtual ~ONSFactory() = default;
  static ONSFactoryAPI* getInstance();
};

} // namespace ons