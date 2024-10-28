#include "react-native-draco.h"
#include "draco/core/decoder_buffer.h"

namespace facebook::react {

ReactNativeDraco::ReactNativeDraco(std::shared_ptr<CallInvoker> jsInvoker)
    : NativeDracoCxxSpec(std::move(jsInvoker)) {}

double ReactNativeDraco::multiply(jsi::Runtime &rt, double a, double b) {
  draco::DecoderBuffer buffer {};
  return a + b;
};

} // namespace facebook::react
