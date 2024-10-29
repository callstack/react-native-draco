#include "react-native-draco.h"
#include "draco/core/decoder_buffer.h"

namespace facebook::react {

class DracoDecoder: public jsi::NativeState {
public:
  DracoDecoder(): decoder_(std::make_unique<draco::Decoder>()), last_status_(std::make_unique<draco::Status>()) {}
  
  std::unique_ptr<draco::Decoder> decoder_;
  std::unique_ptr<draco::Status> last_status_;
};

class DracoDecoderBuffer : public jsi::NativeState {
public:
  DracoDecoderBuffer() : buffer_(std::make_unique<draco::DecoderBuffer>()) {}

  std::unique_ptr<draco::DecoderBuffer> buffer_;
};

class DracoPointCloud: public jsi::NativeState {
public:
  DracoPointCloud(): pointCloud_(std::make_unique<draco::PointCloud>()) {}
  
  std::unique_ptr<draco::PointCloud> pointCloud_;
};

std::shared_ptr<DracoDecoder> tryGetDecoder(jsi::Runtime& rt, jsi::Object& obj) {
  if (!obj.hasNativeState(rt)) {
    return nullptr;
  }

  return std::dynamic_pointer_cast<DracoDecoder>(obj.getNativeState(rt));
}

std::shared_ptr<DracoDecoderBuffer> tryGetDecoderBuffer(jsi::Runtime& rt, jsi::Object& obj) {
  if (!obj.hasNativeState(rt)) {
    return nullptr;
  }

  return std::dynamic_pointer_cast<DracoDecoderBuffer>(obj.getNativeState(rt));
}

std::shared_ptr<DracoPointCloud> tryGetPointCloud(jsi::Runtime& rt, jsi::Object& obj) {
  if (!obj.hasNativeState(rt)) {
    return nullptr;
  }

  return std::dynamic_pointer_cast<DracoPointCloud>(obj.getNativeState(rt));
}

ReactNativeDraco::ReactNativeDraco(std::shared_ptr<CallInvoker> jsInvoker)
: NativeDracoCxxSpec(std::move(jsInvoker)) {}

jsi::Object ReactNativeDraco::createDecoderModule(jsi::Runtime &rt) {
  jsi::Object dracoEncoder{rt};
  dracoEncoder.setNativeState(rt, std::make_shared<DracoDecoder>());
  return dracoEncoder;
}

jsi::Object ReactNativeDraco::createEncoderModule(jsi::Runtime &rt) {
  return jsi::Object(rt);
}

void ReactNativeDraco::attachPointCloudHandle(jsi::Runtime &rt, jsi::Object handle) {
  handle.setNativeState(rt, std::make_shared<DracoPointCloud>());
}

bool ReactNativeDraco::GetAttributeDataArrayForAllPoints(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object pointCloudHandle, jsi::Object pointAttributeHandle, NativeDracoDataType dataType, int outSize, jsi::Object outValues) {
  return false;
}

void ReactNativeDraco::SkipAttributeTransform(jsi::Runtime &rt, NativeDracoGeometryAttribute attributeType) {
}

void ReactNativeDraco::initBuffer(jsi::Runtime &rt, jsi::Object bufferHandle, jsi::Object data, int length) {
  auto dracoBuffer = std::make_shared<DracoDecoderBuffer>();
  
  if (!data.isArrayBuffer(rt)) {
    throw jsi::JSError(rt, "Data needs to be an array buffer");
  }
  
  auto arrayBuffer = data.getArrayBuffer(rt);
  auto bufferData = arrayBuffer.data(rt);
  
  
  if (bufferData == nullptr) {
    throw jsi::JSError(rt, "Buffer data is null");
  }
  
  dracoBuffer->buffer_->Init((const char*)bufferData, length);
  bufferHandle.setNativeState(rt, dracoBuffer);
}

jsi::Object ReactNativeDraco::GetAttributeByUniqueId(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object pointCloudHandle, int uniqueId) {
  return jsi::Object(rt);
}

jsi::Object ReactNativeDraco::GetAttribute(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object pointCloudHandle, int attributeId) {
  return jsi::Object(rt);
}

bool ReactNativeDraco::GetTrianglesUInt32Array(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object meshHandle, int outSize, jsi::Object outValues) {
  return false;
}

NativeDracoStatus ReactNativeDraco::DecodeBufferToPointCloud(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object bufferHandle, jsi::Object pointCloudHandle) {
  auto decoder = tryGetDecoder(rt, decoderHandle);
  auto buffer = tryGetDecoderBuffer(rt, bufferHandle);
  auto pointCloud = tryGetPointCloud(rt, pointCloudHandle);
  
  auto status = decoder->decoder_->DecodeBufferToGeometry(buffer->buffer_.get(), pointCloud->pointCloud_.get());
  
  
  decoder->last_status_ = std::make_unique<draco::Status>(std::move(status));

  return Bridging<NativeDracoStatus>::fromJs(rt, jsi::Value(status.code()));
}

NativeDracoStatus ReactNativeDraco::DecodeBufferToMesh(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object bufferHandle, jsi::Object meshHandle) {
  return NativeDracoStatus::OK;
}

int ReactNativeDraco::GetAttributeId(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object pointCloudHandle, NativeDracoGeometryAttribute attributeType) {
  return 0;
}

NativeDracoEncodedGeometryType ReactNativeDraco::GetEncodedGeometryType_Deprecated(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object inBuffer) {
  auto decoderBuffer = tryGetDecoderBuffer(rt, inBuffer);
  
  if (decoderBuffer == nullptr) {
    return NativeDracoEncodedGeometryType::INVALID_GEOMETRY_TYPE;
  }
  
  auto value = draco::Decoder::GetEncodedGeometryType(decoderBuffer->buffer_.get()).value();
  
  if (value == draco::INVALID_GEOMETRY_TYPE) {
    return NativeDracoEncodedGeometryType::INVALID_GEOMETRY_TYPE;
  } else if (value == draco::POINT_CLOUD) {
    return NativeDracoEncodedGeometryType::POINT_CLOUD;
  } else if (value == draco::TRIANGULAR_MESH) {
    return NativeDracoEncodedGeometryType::TRIANGULAR_MESH;
  }
 
  return NativeDracoEncodedGeometryType::INVALID_GEOMETRY_TYPE;
}

} // namespace facebook::react
