#pragma once

#include <RNDracoSpecJSI.h>

namespace facebook::react {
class ReactNativeDraco: public NativeDracoCxxSpec<ReactNativeDraco> {
public:
  ReactNativeDraco(std::shared_ptr<CallInvoker> jsInvoker);
  
  double multiply(jsi::Runtime &rt, double a, double b);
};
}

