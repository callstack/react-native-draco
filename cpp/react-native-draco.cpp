#include "react-native-draco.h"
#include "draco/core/decoder_buffer.h"
#include "draco-state.h"
#include "draco-helpers.h"
#include <thread>
#include <future>

namespace facebook::react {

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
    auto mesh = tryGetDracoObject<DracoMesh>(rt, thisHandle);
    auto faces = mesh->mesh_.num_faces();
    return jsi::Value((int)faces);
  });
  
  auto num_attributes = HOSTFN("num_attributes", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetDracoObject<DracoMesh>(rt, thisHandle);
    auto attributes = mesh->mesh_.num_attributes();
    return jsi::Value((int)attributes);
  });
  
  auto num_points = HOSTFN("num_points", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetDracoObject<DracoMesh>(rt, thisHandle);
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
    auto mesh = tryGetDracoObject<DracoPointCloud>(rt, thisHandle);
    auto points = mesh->pointCloud_.num_points();
    return jsi::Value((int)points);
  });
  
  auto num_attributes = HOSTFN("num_attributes", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetDracoObject<DracoPointCloud>(rt, thisHandle);
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
    auto mesh = tryGetDracoObject<DracoPointAttribute>(rt, thisHandle);
    auto value = mesh->pointAttribute_.size();
    return jsi::Value((int)value);
  });
  
  auto attribute_type = HOSTFN("attribute_type", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetDracoObject<DracoPointAttribute>(rt, thisHandle);
    auto value = mesh->pointAttribute_.attribute_type();
    return jsi::Value((int)value);
  });
  
  auto data_type = HOSTFN("data_type", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetDracoObject<DracoPointAttribute>(rt, thisHandle);
    auto value = mesh->pointAttribute_.data_type();
    return jsi::Value((int)value);
  });
  
  auto normalized = HOSTFN("normalized", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetDracoObject<DracoPointAttribute>(rt, thisHandle);
    auto value = mesh->pointAttribute_.normalized();
    return jsi::Value((bool)value);
  });
  
  auto byte_stride = HOSTFN("byte_stride", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetDracoObject<DracoPointAttribute>(rt, thisHandle);
    auto value = mesh->pointAttribute_.byte_stride();
    return jsi::Value((int)value);
  });
  
  auto byte_offset = HOSTFN("byte_offset", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetDracoObject<DracoPointAttribute>(rt, thisHandle);
    auto value = mesh->pointAttribute_.byte_offset();
    return jsi::Value((int)value);
  });
  
  auto unique_id = HOSTFN("unique_id", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetDracoObject<DracoPointAttribute>(rt, thisHandle);
    auto value = mesh->pointAttribute_.unique_id();
    return jsi::Value((int)value);
  });
  
  auto num_components = HOSTFN("num_components", 0, []) {
    auto thisHandle = thisValue.getObject(rt);
    auto mesh = tryGetDracoObject<DracoPointAttribute>(rt, thisHandle);
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
  auto decoder = tryGetDracoObject<DracoDecoder>(rt, decoderHandle);
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
  auto pointCloud = tryGetDracoObject<DracoPointCloud>(rt, pointCloudHandle);
  auto mesh = tryGetDracoObject<DracoMesh>(rt, pointCloudHandle);
  
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
  
  auto pointCloud = tryGetDracoObject<DracoPointCloud>(rt, pointCloudHandle);
  auto mesh = tryGetDracoObject<DracoMesh>(rt, pointCloudHandle);
  
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
  auto pointCloud = tryGetDracoObject<DracoPointCloud>(rt, pointCloudHandle);
  auto mesh = tryGetDracoObject<DracoMesh>(rt, pointCloudHandle);
  auto geometryAttr = draco::GeometryAttribute::Type((int)attributeType);
  
  auto &object = (pointCloud != nullptr) ? pointCloud->pointCloud_ : mesh->mesh_;
  
  return object.GetNamedAttributeId(geometryAttr);
}

bool ReactNativeDraco::GetTrianglesUInt32Array(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object meshHandle, int outSize, jsi::Object outValues) {
  auto mesh = tryGetDracoObject<DracoMesh>(rt, meshHandle);
  
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
  auto pc = tryGetDracoObject<DracoPointCloud>(rt, pointCloudHandle);
  auto mesh = tryGetDracoObject<DracoMesh>(rt, pointCloudHandle);
  auto pa = tryGetDracoObject<DracoPointAttribute>(rt, pointAttributeHandle);
  
  if (!outValues.isArrayBuffer(rt)) {
    throw jsi::JSError(rt, "Data needs to be an array buffer");
  }
  
  auto arrayBuffer = outValues.getArrayBuffer(rt);
  auto bufferData = arrayBuffer.data(rt);
  
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
  auto decoder = tryGetDracoObject<DracoDecoder>(rt, decoderHandle);
  auto buffer = tryGetDracoObject<DracoDecoderBuffer>(rt, bufferHandle);
  auto pointCloud = tryGetDracoObject<DracoPointCloud>(rt, pointCloudHandle);
  
  std::promise<draco::Status> resultPromise;
  std::future<draco::Status> resultFuture = resultPromise.get_future();
  
  std::thread decodingThread([&buffer, &pointCloud, &decoder, promise = std::move(resultPromise)]() mutable {
    try {
      auto status = decoder->decoder_.DecodeBufferToGeometry(&buffer->buffer_, &pointCloud->pointCloud_);
      decoder->last_status_ = draco::Status(std::move(status));
      promise.set_value(status);
    } catch (...) {
      promise.set_exception(std::current_exception());
    }
    
  });
  
  decodingThread.detach();
  
  try {
    auto status = resultFuture.get();
    return Bridging<NativeDracoStatus>::fromJs(rt, jsi::Value(status.code()));
  } catch (const std::exception& e) {
    throw jsi::JSError(rt, e.what());
  }
}

NativeDracoStatus ReactNativeDraco::DecodeBufferToMesh(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object bufferHandle, jsi::Object meshHandle) {
  auto decoder = tryGetDracoObject<DracoDecoder>(rt, decoderHandle);
  auto buffer = tryGetDracoObject<DracoDecoderBuffer>(rt, bufferHandle);
  auto mesh = tryGetDracoObject<DracoMesh>(rt, meshHandle);
  
  std::promise<draco::Status> resultPromise;
  std::future<draco::Status> resultFuture = resultPromise.get_future();
  
  std::thread decodingThread([&buffer, &mesh, &decoder, promise = std::move(resultPromise)]() mutable {
    try {
      auto status = decoder->decoder_.DecodeBufferToGeometry(&buffer->buffer_, &mesh->mesh_);
      decoder->last_status_ = draco::Status(std::move(status));
      promise.set_value(status);
    } catch (...) {
      promise.set_exception(std::current_exception());
    }
    
  });
  
  decodingThread.detach();
  
  try {
    auto status = resultFuture.get();
    return Bridging<NativeDracoStatus>::fromJs(rt, jsi::Value(status.code()));
  } catch (const std::exception& e) {
    throw jsi::JSError(rt, e.what());
  }
}


NativeDracoEncodedGeometryType ReactNativeDraco::GetEncodedGeometryType_Deprecated(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object inBuffer) {
  auto decoderBuffer = tryGetDracoObject<DracoDecoderBuffer>(rt, inBuffer);
  
  if (decoderBuffer == nullptr) {
    return NativeDracoEncodedGeometryType::INVALID_GEOMETRY_TYPE;
  }
  
  auto value = draco::Decoder::GetEncodedGeometryType(&decoderBuffer->buffer_).value();
  return Bridging<NativeDracoEncodedGeometryType>::fromJs(rt, jsi::Value(value));
}

} // namespace facebook::react
