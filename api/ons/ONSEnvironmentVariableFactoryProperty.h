#pragma once

#include "ONSFactoryProperty.h"

namespace ons {

class ONSEnvironmentVariableFactoryProperty : public ONSFactoryProperty {
public:
  ONSEnvironmentVariableFactoryProperty();

  ~ONSEnvironmentVariableFactoryProperty() override = default;

private:
  void parseEnvironmentVariables();
};

} // namespace ons