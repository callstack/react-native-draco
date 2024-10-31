#include "react-native-draco.h"
#include "draco/core/decoder_buffer.h"

#define HOSTFN(name, size, capture) \
jsi::Function::createFromHostFunction(rt, jsi::PropNameID::forAscii(rt, name), size, \
capture(jsi::Runtime &rt, const jsi::Value &thisValue, \
const jsi::Value *arguments, size_t count)          \
->jsi::Value

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

class DracoPointAttribute: public jsi::NativeState {
public:
  DracoPointAttribute(): pointAttribute_(std::make_unique<draco::PointAttribute>()) {}
  
  std::unique_ptr<draco::PointAttribute> pointAttribute_;
};

class DracoMesh: public jsi::NativeState {
public:
  DracoMesh(): mesh_(std::make_unique<draco::Mesh>()) {}
  
  std::unique_ptr<draco::Mesh> mesh_;
};

std::shared_ptr<DracoDecoder> tryGetDecoder(jsi::Runtime& rt, jsi::Object& obj) {
  if (!obj.hasNativeState(rt)) {
    return nullptr;
  }
  
  return std::dynamic_pointer_cast<DracoDecoder>(obj.getNativeState(rt));
}

std::shared_ptr<DracoPointAttribute> tryGetPointAttribute(jsi::Runtime& rt, jsi::Object& obj) {
  if (!obj.hasNativeState(rt)) {
    return nullptr;
  }
  
  return std::dynamic_pointer_cast<DracoPointAttribute>(obj.getNativeState(rt));
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

std::shared_ptr<DracoMesh> tryGetMesh(jsi::Runtime& rt, jsi::Object& obj) {
  if (!obj.hasNativeState(rt)) {
    return nullptr;
  }
  
  return std::dynamic_pointer_cast<DracoMesh>(obj.getNativeState(rt));
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

void ReactNativeDraco::attachMeshHandle(jsi::Runtime &rt, jsi::Object handle) {
  handle.setNativeState(rt, std::make_shared<DracoMesh>());
}

void ReactNativeDraco::attachPointAttributeHandle(jsi::Runtime &rt, jsi::Object handle) {
  handle.setNativeState(rt, std::make_shared<DracoPointAttribute>());
}

void ReactNativeDraco::installMeshMethods(jsi::Runtime &rt, jsi::Object handle) {
  auto num_faces = HOSTFN("num_faces", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetMesh(rt, thisHandle);
    auto faces = mesh->mesh_->num_faces();
    return jsi::Value((int)faces);
  });
  
  auto num_attributes = HOSTFN("num_attributes", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetMesh(rt, thisHandle);
    auto attributes = mesh->mesh_->num_attributes();
    return jsi::Value((int)attributes);
  });
  
  auto num_points = HOSTFN("num_points", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetMesh(rt, thisHandle);
    auto points = mesh->mesh_->num_points();
    return jsi::Value((int)points);
  });
  
  handle.setProperty(rt, "num_faces", std::move(num_faces));
  handle.setProperty(rt, "num_points", std::move(num_points));
  handle.setProperty(rt, "num_attributes", std::move(num_attributes));
}

void ReactNativeDraco::installPointCloudMethods(jsi::Runtime &rt, jsi::Object handle) {
  auto num_points = HOSTFN("num_points", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetPointCloud(rt, thisHandle);
    auto points = mesh->pointCloud_->num_points();
    return jsi::Value((int)points);
  });
  
  auto num_attributes = HOSTFN("num_attributes", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetPointCloud(rt, thisHandle);
    auto attributes = mesh->pointCloud_->num_attributes();
    return jsi::Value((int)attributes);
  });
  
  handle.setProperty(rt, "num_points", std::move(num_points));
  handle.setProperty(rt, "num_attributes", std::move(num_attributes));
}

void ReactNativeDraco::installPointAttributeMethods(jsi::Runtime &rt, jsi::Object handle) {
  auto size = HOSTFN("size", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetPointAttribute(rt, thisHandle);
    auto value = mesh->pointAttribute_->size();
    return jsi::Value((int)value);
  });
  
  auto attribute_type = HOSTFN("attribute_type", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetPointAttribute(rt, thisHandle);
    auto value = mesh->pointAttribute_->attribute_type();
    return jsi::Value((int)value);
  });
  
  auto data_type = HOSTFN("data_type", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetPointAttribute(rt, thisHandle);
    auto value = mesh->pointAttribute_->data_type();
    return jsi::Value((int)value);
  });
  
  auto normalized = HOSTFN("normalized", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetPointAttribute(rt, thisHandle);
    auto value = mesh->pointAttribute_->normalized();
    return jsi::Value((bool)value);
  });
  
  auto byte_stride = HOSTFN("byte_stride", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetPointAttribute(rt, thisHandle);
    auto value = mesh->pointAttribute_->byte_stride();
    return jsi::Value((int)value);
  });
  
  auto byte_offset = HOSTFN("byte_offset", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetPointAttribute(rt, thisHandle);
    auto value = mesh->pointAttribute_->byte_offset();
    return jsi::Value((int)value);
  });
  
  auto unique_id = HOSTFN("unique_id", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetPointAttribute(rt, thisHandle);
    auto value = mesh->pointAttribute_->unique_id();
    return jsi::Value((int)value);
  });
  
  handle.setProperty(rt, "size", std::move(size));
  handle.setProperty(rt, "attribute_type", std::move(attribute_type));
  handle.setProperty(rt, "data_type", std::move(data_type));
  handle.setProperty(rt, "normalized", std::move(normalized));
  handle.setProperty(rt, "byte_stride", std::move(byte_stride));
  handle.setProperty(rt, "byte_offset", std::move(byte_offset));
  handle.setProperty(rt, "unique_id", std::move(unique_id));
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
  auto decoder = tryGetDecoder(rt, decoderHandle);
  auto buffer = tryGetDecoderBuffer(rt, bufferHandle);
  auto mesh = tryGetMesh(rt, meshHandle);
  
  auto status = decoder->decoder_->DecodeBufferToGeometry(buffer->buffer_.get(), mesh->mesh_.get());
  decoder->last_status_ = std::make_unique<draco::Status>(std::move(status));
  
  return Bridging<NativeDracoStatus>::fromJs(rt, jsi::Value(status.code()));
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
