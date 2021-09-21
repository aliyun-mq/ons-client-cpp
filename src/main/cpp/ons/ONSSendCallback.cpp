#include "ONSSendCallback.h"

namespace ons {

absl::Mutex ONSSendCallback::mutex_;

ONSSendCallback* ONSSendCallback::instance_ = nullptr;

} // namespace ons