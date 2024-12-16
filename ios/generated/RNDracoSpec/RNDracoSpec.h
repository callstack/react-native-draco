/**
 * This code was generated by [react-native-codegen](https://www.npmjs.com/package/react-native-codegen).
 *
 * Do not edit this file as changes may cause incorrect behavior and will be lost
 * once the code is regenerated.
 *
 * @generated by codegen project: GenerateModuleObjCpp
 *
 * We create an umbrella header (and corresponding implementation) here since
 * Cxx compilation in BUCK has a limitation: source-code producing genrule()s
 * must have a single output. More files => more genrule()s => slower builds.
 */

#ifndef __cplusplus
#error This file must be compiled as Obj-C++. If you are importing it, you must change your file extension to .mm.
#endif

// Avoid multiple includes of RNDracoSpec symbols
#ifndef RNDracoSpec_H
#define RNDracoSpec_H

#import <Foundation/Foundation.h>
#import <RCTRequired/RCTRequired.h>
#import <RCTTypeSafety/RCTConvertHelpers.h>
#import <RCTTypeSafety/RCTTypedModuleConstants.h>
#import <React/RCTBridgeModule.h>
#import <React/RCTCxxConvert.h>
#import <React/RCTManagedPointer.h>
#import <ReactCommon/RCTTurboModule.h>
#import <optional>
#import <vector>


@protocol NativeDracoSpec <RCTBridgeModule, RCTTurboModule>

- (NSDictionary *)createDecoderModule;
- (NSDictionary *)createEncoderModule;
- (void)attachPointCloudHandle:(NSDictionary *)handle;
- (void)attachMeshHandle:(NSDictionary *)handle;
- (void)attachPointAttributeHandle:(NSDictionary *)handle;
- (void)installMeshMethods:(NSDictionary *)handle;
- (void)installPointCloudMethods:(NSDictionary *)handle;
- (void)installPointAttributeMethods:(NSDictionary *)handle;
- (void)initBuffer:(NSDictionary *)bufferHandle
              data:(NSDictionary *)data
            length:(NSInteger)length;
- (NSNumber *)GetEncodedGeometryType_Deprecated:(NSDictionary *)decoderHandle
                                       inBuffer:(NSDictionary *)inBuffer;
- (NSNumber *)GetAttributeDataArrayForAllPoints:(NSDictionary *)decoderHandle
                               pointCloudHandle:(NSDictionary *)pointCloudHandle
                           pointAttributeHandle:(NSDictionary *)pointAttributeHandle
                                       dataType:(double)dataType
                                        outSize:(NSInteger)outSize
                                      outValues:(NSDictionary *)outValues;
- (void)SkipAttributeTransform:(NSDictionary *)decoderHandle
                 attributeType:(double)attributeType;
- (NSDictionary *)GetAttributeByUniqueId:(NSDictionary *)decoderHandle
                        pointCloudHandle:(NSDictionary *)pointCloudHandle
                                uniqueId:(NSInteger)uniqueId;
- (NSNumber *)GetAttributeId:(NSDictionary *)pointCloudHandle
               attributeType:(double)attributeType;
- (NSDictionary *)GetAttribute:(NSDictionary *)decoderHandle
              pointCloudHandle:(NSDictionary *)pointCloudHandle
                   attributeId:(NSInteger)attributeId;
- (NSNumber *)GetTrianglesUInt32Array:(NSDictionary *)decoderHandle
                           meshHandle:(NSDictionary *)meshHandle
                              outSize:(NSInteger)outSize
                            outValues:(NSDictionary *)outValues;
- (NSNumber *)DecodeBufferToPointCloud:(NSDictionary *)decoderHandle
                          bufferHandle:(NSDictionary *)bufferHandle
                      pointCloudHandle:(NSDictionary *)pointCloudHandle;
- (NSNumber *)DecodeBufferToMesh:(NSDictionary *)decoderHandle
                    bufferHandle:(NSDictionary *)bufferHandle
                      meshHandle:(NSDictionary *)meshHandle;

@end

@interface NativeDracoSpecBase : NSObject {
@protected
facebook::react::EventEmitterCallback _eventEmitterCallback;
}
- (void)setEventEmitterCallback:(EventEmitterCallbackWrapper *)eventEmitterCallbackWrapper;


@end

namespace facebook::react {
  /**
   * ObjC++ class for module 'NativeDraco'
   */
  class JSI_EXPORT NativeDracoSpecJSI : public ObjCTurboModule {
  public:
    NativeDracoSpecJSI(const ObjCTurboModule::InitParams &params);
  };
} // namespace facebook::react

#endif // RNDracoSpec_H