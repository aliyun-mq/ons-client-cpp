#include "ons/ONSFactory.h"

#include "ONSFactoryInstance.h"

namespace ons {

ONSFactoryAPI* ONSFactory::getInstance() {
  static ONSFactoryInstance instance_;
  return &instance_;
}

} // namespace ons