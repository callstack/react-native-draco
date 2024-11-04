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
  DracoDecoder(): decoder_(draco::Decoder()), last_status_(draco::Status()) {}
  
  draco::Decoder decoder_;
  draco::Status last_status_;
};

class DracoDecoderBuffer : public jsi::NativeState {
public:
  DracoDecoderBuffer() : buffer_(draco::DecoderBuffer()) {}
  
  draco::DecoderBuffer buffer_;
};

class DracoPointCloud: public jsi::NativeState {
public:
  DracoPointCloud(): pointCloud_(draco::PointCloud()) {}
  
  draco::PointCloud pointCloud_;
};

class DracoPointAttribute: public jsi::NativeState {
public:
  DracoPointAttribute(): pointAttribute_(draco::PointAttribute()) {}
  
  explicit DracoPointAttribute(const draco::PointAttribute &attribute): pointAttribute_(draco::PointAttribute()) {
    pointAttribute_.CopyFrom(attribute);
  }
  
  explicit DracoPointAttribute(draco::PointAttribute &&attribute): pointAttribute_(std::move(attribute)) {}
  
  draco::PointAttribute pointAttribute_;
};

class DracoMesh: public jsi::NativeState {
public:
  DracoMesh(): mesh_(draco::Mesh()) {}
  
  draco::Mesh mesh_;
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

template <typename T>
bool GetTrianglesArray(const draco::Mesh &m, const int out_size,
                       T *out_values) {
  const uint32_t num_faces = m.num_faces();
  if (num_faces * 3 * sizeof(T) != out_size) {
    return false;
  }
  
  for (uint32_t face_id = 0; face_id < num_faces; ++face_id) {
    const draco::Mesh::Face &face = m.face(draco::FaceIndex(face_id));
    out_values[face_id * 3 + 0] = static_cast<T>(face[0].value());
    out_values[face_id * 3 + 1] = static_cast<T>(face[1].value());
    out_values[face_id * 3 + 2] = static_cast<T>(face[2].value());
  }
  return true;
}

template <class T>
static bool GetAttributeDataArrayForAllPointsHelper(const draco::PointCloud &pc,
                                                    const draco::PointAttribute &pa,
                                                    const draco::DataType type,
                                                    int out_size,
                                                    void *out_values) {
  const int components = pa.num_components();
  const int num_points = pc.num_points();
  const int data_size = num_points * components * sizeof(T);
  if (data_size != out_size) {
    return false;
  }
  const bool requested_type_matches = pa.data_type() == type;
  if (requested_type_matches && pa.is_mapping_identity()) {
    // Copy values directly to the output vector.
    const auto ptr = pa.GetAddress(draco::AttributeValueIndex(0));
    ::memcpy(out_values, ptr, data_size);
    return true;
  }
  
  return false;
};

bool GetAttributeFloatArrayForAllPoints(const draco::PointCloud &pc,
                                        const draco::PointAttribute &pa,
                                        int out_size,
                                        void *out_values) {
  const int components = pa.num_components();
  const int num_points = pc.num_points();
  const int data_size = num_points * components * sizeof(float);
  if (data_size != out_size) {
    return false;
  }
  const bool requested_type_is_float = pa.data_type() == draco::DT_FLOAT32;
  std::vector<float> values(components, -2.f);
  int entry_id = 0;
  float *const floats = reinterpret_cast<float *>(out_values);
  
  for (draco::PointIndex i(0); i < num_points; ++i) {
    const draco::AttributeValueIndex val_index = pa.mapped_index(i);
    if (requested_type_is_float) {
      pa.GetValue(val_index, &values[0]);
    } else {
      if (!pa.ConvertValue<float>(val_index, &values[0])) {
        return false;
      }
    }
    for (int j = 0; j < components; ++j) {
      floats[entry_id++] = values[j];
    }
  }
  return true;
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
  _installMeshMethods(rt, handle);
}

void ReactNativeDraco::_installMeshMethods(jsi::Runtime &rt, jsi::Object& handle) {
  auto num_faces = HOSTFN("num_faces", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetMesh(rt, thisHandle);
    auto faces = mesh->mesh_.num_faces();
    return jsi::Value((int)faces);
  });
  
  auto num_attributes = HOSTFN("num_attributes", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetMesh(rt, thisHandle);
    auto attributes = mesh->mesh_.num_attributes();
    return jsi::Value((int)attributes);
  });
  
  auto num_points = HOSTFN("num_points", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetMesh(rt, thisHandle);
    auto points = mesh->mesh_.num_points();
    return jsi::Value((int)points);
  });
  
  handle.setProperty(rt, "num_faces", std::move(num_faces));
  handle.setProperty(rt, "num_points", std::move(num_points));
  handle.setProperty(rt, "num_attributes", std::move(num_attributes));
}

void ReactNativeDraco::installPointCloudMethods(jsi::Runtime &rt, jsi::Object handle) {
  _installPointCloudMethods(rt, handle);
}

void ReactNativeDraco::_installPointCloudMethods(jsi::Runtime &rt, jsi::Object& handle) {
  auto num_points = HOSTFN("num_points", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetPointCloud(rt, thisHandle);
    auto points = mesh->pointCloud_.num_points();
    return jsi::Value((int)points);
  });
  
  auto num_attributes = HOSTFN("num_attributes", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetPointCloud(rt, thisHandle);
    auto attributes = mesh->pointCloud_.num_attributes();
    return jsi::Value((int)attributes);
  });
  
  handle.setProperty(rt, "num_points", std::move(num_points));
  handle.setProperty(rt, "num_attributes", std::move(num_attributes));
}

void ReactNativeDraco::installPointAttributeMethods(jsi::Runtime &rt, jsi::Object handle) {
  _installPointAttributeMethods(rt, handle);
}

void ReactNativeDraco::_installPointAttributeMethods(jsi::Runtime &rt, jsi::Object& handle) {
  auto size = HOSTFN("size", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetPointAttribute(rt, thisHandle);
    auto value = mesh->pointAttribute_.size();
    return jsi::Value((int)value);
  });
  
  auto attribute_type = HOSTFN("attribute_type", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetPointAttribute(rt, thisHandle);
    auto value = mesh->pointAttribute_.attribute_type();
    return jsi::Value((int)value);
  });
  
  auto data_type = HOSTFN("data_type", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetPointAttribute(rt, thisHandle);
    auto value = mesh->pointAttribute_.data_type();
    return jsi::Value((int)value);
  });
  
  auto normalized = HOSTFN("normalized", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetPointAttribute(rt, thisHandle);
    auto value = mesh->pointAttribute_.normalized();
    return jsi::Value((bool)value);
  });
  
  auto byte_stride = HOSTFN("byte_stride", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetPointAttribute(rt, thisHandle);
    auto value = mesh->pointAttribute_.byte_stride();
    return jsi::Value((int)value);
  });
  
  auto byte_offset = HOSTFN("byte_offset", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetPointAttribute(rt, thisHandle);
    auto value = mesh->pointAttribute_.byte_offset();
    return jsi::Value((int)value);
  });
  
  auto unique_id = HOSTFN("unique_id", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetPointAttribute(rt, thisHandle);
    auto value = mesh->pointAttribute_.unique_id();
    return jsi::Value((int)value);
  });
  
  auto num_components = HOSTFN("num_components", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetPointAttribute(rt, thisHandle);
    auto value = mesh->pointAttribute_.num_components();
    return jsi::Value((int)value);
  });
  
  handle.setProperty(rt, "size", std::move(size));
  handle.setProperty(rt, "attribute_type", std::move(attribute_type));
  handle.setProperty(rt, "data_type", std::move(data_type));
  handle.setProperty(rt, "normalized", std::move(normalized));
  handle.setProperty(rt, "byte_stride", std::move(byte_stride));
  handle.setProperty(rt, "byte_offset", std::move(byte_offset));
  handle.setProperty(rt, "unique_id", std::move(unique_id));
  handle.setProperty(rt, "num_components", std::move(num_components));
}

void ReactNativeDraco::SkipAttributeTransform(jsi::Runtime &rt, jsi::Object decoderHandle, NativeDracoGeometryAttribute attributeType) {
  auto decoder = tryGetDecoder(rt, decoderHandle);
  decoder->decoder_.SetSkipAttributeTransform(draco::GeometryAttribute::Type((int)attributeType));
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
  
  dracoBuffer->buffer_.Init((const char*)bufferData, length);
  bufferHandle.setNativeState(rt, dracoBuffer);
}

jsi::Object ReactNativeDraco::GetAttributeByUniqueId(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object pointCloudHandle, int uniqueId) {
  auto pointCloud = tryGetPointCloud(rt, pointCloudHandle);
  auto mesh = tryGetMesh(rt, pointCloudHandle);
  
  const draco::PointAttribute *pointAttr = nullptr;
  
  try {
    auto &pc = (pointCloud != nullptr) ? pointCloud->pointCloud_ : mesh->mesh_;
    pointAttr = pc.GetAttributeByUniqueId(uniqueId);
  } catch (std::exception& e) {
    throw jsi::JSError(rt, "Operation failed");
  }
  
  if (pointAttr == nullptr) {
    throw jsi::JSError(rt, "Failed to get attribute by unique id");
  }
  
  auto object = jsi::Object{rt};
  object.setNativeState(rt, std::make_shared<DracoPointAttribute>(*pointAttr));
  _installPointAttributeMethods(rt, object);
  
  return object;
}

jsi::Object ReactNativeDraco::GetAttribute(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object pointCloudHandle, int attributeId) {
  
  auto pointCloud = tryGetPointCloud(rt, pointCloudHandle);
  auto mesh = tryGetMesh(rt, pointCloudHandle);
  
  const draco::PointAttribute *pointAttr = nullptr;
  
  try {
    auto &pc = (pointCloud != nullptr) ? pointCloud->pointCloud_ : mesh->mesh_;
    pointAttr = pc.attribute(attributeId);
  } catch (std::exception& e) {
    throw jsi::JSError(rt, "Operation failed");
  }
  
  if (pointAttr == nullptr) {
    throw jsi::JSError(rt, "Failed to get attribute by unique id");
  }
  
  auto object = jsi::Object{rt};
  object.setNativeState(rt, std::make_shared<DracoPointAttribute>(*pointAttr));
  _installPointAttributeMethods(rt, object);
  
  return object;
}

int ReactNativeDraco::GetAttributeId(jsi::Runtime &rt, jsi::Object pointCloudHandle, NativeDracoGeometryAttribute attributeType) {
  auto pointCloud = tryGetPointCloud(rt, pointCloudHandle);
  auto mesh = tryGetMesh(rt, pointCloudHandle);
  auto geometryAttr = draco::GeometryAttribute::Type((int)attributeType);
  
  auto &object = (pointCloud != nullptr) ? pointCloud->pointCloud_ : mesh->mesh_;
  
  return object.GetNamedAttributeId(geometryAttr);
}

bool ReactNativeDraco::GetTrianglesUInt32Array(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object meshHandle, int outSize, jsi::Object outValues) {
  auto mesh = tryGetMesh(rt, meshHandle);
  
  if (!outValues.isArrayBuffer(rt)) {
    throw jsi::JSError(rt, "Data needs to be an array buffer");
  }
  
  auto arrayBuffer = outValues.getArrayBuffer(rt);
  auto bufferData = arrayBuffer.data(rt);
  
  return GetTrianglesArray<uint32_t>(mesh->mesh_, outSize,
                                     reinterpret_cast<uint32_t *>(bufferData));
  return false;
}

bool ReactNativeDraco::GetAttributeDataArrayForAllPoints(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object pointCloudHandle, jsi::Object pointAttributeHandle, NativeDracoDataType dataType, int outSize, jsi::Object outValues) {
  auto pc = tryGetPointCloud(rt, pointCloudHandle);
  auto mesh = tryGetMesh(rt, pointCloudHandle);
  auto pa = tryGetPointAttribute(rt, pointAttributeHandle);
  
  if (!outValues.isArrayBuffer(rt)) {
    throw jsi::JSError(rt, "Data needs to be an array buffer");
  }
  
  auto arrayBuffer = outValues.getArrayBuffer(rt);
  auto bufferData = arrayBuffer.data(rt);
  
  // TODO: Properly handle inheritance of PointCloud and Mesh, currently due to NativeState wrappers its broken.
  try {
    auto &object = (pc != nullptr) ? pc->pointCloud_ : mesh->mesh_;
    switch (dataType) {
      case NativeDracoDataType::DT_INT8:
        return GetAttributeDataArrayForAllPointsHelper<int8_t>(object, pa->pointAttribute_, draco::DT_INT8,
                                                               outSize, bufferData);
      case NativeDracoDataType::DT_INT16:
        return GetAttributeDataArrayForAllPointsHelper<int16_t>(object, pa->pointAttribute_, draco::DT_INT8,
                                                                outSize, bufferData);
      case NativeDracoDataType::DT_INT32:
        return GetAttributeDataArrayForAllPointsHelper<int32_t>(object, pa->pointAttribute_, draco::DT_INT8,
                                                                outSize, bufferData);
      case NativeDracoDataType::DT_UINT8:
        return GetAttributeDataArrayForAllPointsHelper<uint8_t>(object, pa->pointAttribute_, draco::DT_INT8,
                                                                outSize, bufferData);
      case NativeDracoDataType::DT_UINT16:
        return GetAttributeDataArrayForAllPointsHelper<uint16_t>(object, pa->pointAttribute_, draco::DT_INT8,
                                                                 outSize, bufferData);
      case NativeDracoDataType::DT_UINT32:
        return GetAttributeDataArrayForAllPointsHelper<uint32_t>(object, pa->pointAttribute_, draco::DT_INT8,
                                                                 outSize, bufferData);
      case NativeDracoDataType::DT_FLOAT32:
        return GetAttributeFloatArrayForAllPoints(object, pa->pointAttribute_, outSize, bufferData);
      default:
        return false;
    }
  } catch (std::exception e) {
    throw jsi::JSError(rt, "Operation failed");
  }
  
  return false;
}

NativeDracoStatus ReactNativeDraco::DecodeBufferToPointCloud(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object bufferHandle, jsi::Object pointCloudHandle) {
  auto decoder = tryGetDecoder(rt, decoderHandle);
  auto buffer = tryGetDecoderBuffer(rt, bufferHandle);
  auto pointCloud = tryGetPointCloud(rt, pointCloudHandle);
  
  auto status = decoder->decoder_.DecodeBufferToGeometry(&buffer->buffer_, &pointCloud->pointCloud_);
  decoder->last_status_ = draco::Status(std::move(status));
  
  return Bridging<NativeDracoStatus>::fromJs(rt, jsi::Value(status.code()));
}

NativeDracoStatus ReactNativeDraco::DecodeBufferToMesh(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object bufferHandle, jsi::Object meshHandle) {
  auto decoder = tryGetDecoder(rt, decoderHandle);
  auto buffer = tryGetDecoderBuffer(rt, bufferHandle);
  auto mesh = tryGetMesh(rt, meshHandle);
  
  auto status = decoder->decoder_.DecodeBufferToGeometry(&buffer->buffer_, &mesh->mesh_);
  decoder->last_status_ = draco::Status(std::move(status));
  
  return Bridging<NativeDracoStatus>::fromJs(rt, jsi::Value(status.code()));
}


NativeDracoEncodedGeometryType ReactNativeDraco::GetEncodedGeometryType_Deprecated(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object inBuffer) {
  auto decoderBuffer = tryGetDecoderBuffer(rt, inBuffer);
  
  if (decoderBuffer == nullptr) {
    return NativeDracoEncodedGeometryType::INVALID_GEOMETRY_TYPE;
  }
  
  auto value = draco::Decoder::GetEncodedGeometryType(&decoderBuffer->buffer_).value();
  
  return Bridging<NativeDracoEncodedGeometryType>::fromJs(rt, jsi::Value(value));
}

} // namespace facebook::react
