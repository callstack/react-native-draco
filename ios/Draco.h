#ifdef __cplusplus
#import "react-native-draco.h"
#endif

#ifdef RCT_NEW_ARCH_ENABLED
#import "RNDracoSpec.h"

@interface Draco : NSObject <NativeDracoSpec>
#else
#import <React/RCTBridgeModule.h>

@interface Draco : NSObject <RCTBridgeModule>
#endif

@end
