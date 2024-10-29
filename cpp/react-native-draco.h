#pragma once

#include <RNDracoSpecJSI.h>
#include <jsi/jsi.h>

#include "draco/attributes/attribute_transform_type.h"
#include "draco/attributes/point_attribute.h"
#include "draco/compression/config/compression_shared.h"
#include "draco/compression/decode.h"
#include "draco/core/decoder_buffer.h"
#include "draco/mesh/mesh.h"

namespace facebook::react {



class ReactNativeDraco: public NativeDracoCxxSpec<ReactNativeDraco> {
public:
  ReactNativeDraco(std::shared_ptr<CallInvoker> jsInvoker);
 
  NativeDracoEncodedGeometryType GetEncodedGeometryType_Deprecated(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object inBuffer);
  
  jsi::Object createDecoderModule(jsi::Runtime &rt);
  jsi::Object createEncoderModule(jsi::Runtime &rt);
  void attachPointCloudHandle(jsi::Runtime &rt, jsi::Object handle);
  bool GetAttributeDataArrayForAllPoints(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object pointCloudHandle, jsi::Object pointAttributeHandle, NativeDracoDataType dataType, int outSize, jsi::Object outValues);
  void SkipAttributeTransform(jsi::Runtime &rt, NativeDracoGeometryAttribute attributeType);
  jsi::Object GetAttributeByUniqueId(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object pointCloudHandle, int uniqueId);
  jsi::Object GetAttribute(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object pointCloudHandle, int attributeId);
  bool GetTrianglesUInt32Array(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object meshHandle, int outSize, jsi::Object outValues);
  NativeDracoStatus DecodeBufferToPointCloud(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object bufferHandle, jsi::Object pointCloudHandle);
  NativeDracoStatus DecodeBufferToMesh(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object bufferHandle, jsi::Object meshHandle);
  int GetAttributeId(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object pointCloudHandle, NativeDracoGeometryAttribute attributeType);
  void initBuffer(jsi::Runtime &rt, jsi::Object bufferHandle, jsi::Object data, int length);
};
}

