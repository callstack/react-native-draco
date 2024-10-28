#import "Draco.h"

#include <ReactCommon/CxxTurboModuleUtils.h>

@implementation Draco

+ (void)load {
   facebook::react::registerCxxModuleToGlobalModuleMap(
   std::string(facebook::react::ReactNativeDraco::kModuleName),
   [&](std::shared_ptr<facebook::react::CallInvoker> jsInvoker) {
   return std::make_shared<facebook::react::ReactNativeDraco>(jsInvoker);
 });
}

@end
