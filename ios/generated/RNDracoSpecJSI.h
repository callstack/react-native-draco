/**
 * This code was generated by [react-native-codegen](https://www.npmjs.com/package/react-native-codegen).
 *
 * Do not edit this file as changes may cause incorrect behavior and will be lost
 * once the code is regenerated.
 *
 * @generated by codegen project: GenerateModuleH.js
 */

#pragma once

#include <ReactCommon/TurboModule.h>
#include <react/bridging/Bridging.h>

namespace facebook::react {


#pragma mark - NativeDracoDataType

enum class NativeDracoDataType { DT_INVALID, DT_INT8, DT_UINT8, DT_INT16, DT_UINT16, DT_INT32, DT_UINT32, DT_INT64, DT_UINT64, DT_FLOAT32, DT_FLOAT64, DT_BOOL, DT_TYPES_COUNT };

template <>
struct Bridging<NativeDracoDataType> {
  static NativeDracoDataType fromJs(jsi::Runtime &rt, const jsi::Value &rawValue) {
    double value = (double)rawValue.asNumber();
    if (value == 0) {
      return NativeDracoDataType::DT_INVALID;
    } else if (value == 1) {
      return NativeDracoDataType::DT_INT8;
    } else if (value == 2) {
      return NativeDracoDataType::DT_UINT8;
    } else if (value == 3) {
      return NativeDracoDataType::DT_INT16;
    } else if (value == 4) {
      return NativeDracoDataType::DT_UINT16;
    } else if (value == 5) {
      return NativeDracoDataType::DT_INT32;
    } else if (value == 6) {
      return NativeDracoDataType::DT_UINT32;
    } else if (value == 7) {
      return NativeDracoDataType::DT_INT64;
    } else if (value == 8) {
      return NativeDracoDataType::DT_UINT64;
    } else if (value == 9) {
      return NativeDracoDataType::DT_FLOAT32;
    } else if (value == 10) {
      return NativeDracoDataType::DT_FLOAT64;
    } else if (value == 11) {
      return NativeDracoDataType::DT_BOOL;
    } else if (value == 12) {
      return NativeDracoDataType::DT_TYPES_COUNT;
    } else {
      throw jsi::JSError(rt, "No appropriate enum member found for value");
    }
  }

  static jsi::Value toJs(jsi::Runtime &rt, NativeDracoDataType value) {
    if (value == NativeDracoDataType::DT_INVALID) {
      return bridging::toJs(rt, 0);
    } else if (value == NativeDracoDataType::DT_INT8) {
      return bridging::toJs(rt, 1);
    } else if (value == NativeDracoDataType::DT_UINT8) {
      return bridging::toJs(rt, 2);
    } else if (value == NativeDracoDataType::DT_INT16) {
      return bridging::toJs(rt, 3);
    } else if (value == NativeDracoDataType::DT_UINT16) {
      return bridging::toJs(rt, 4);
    } else if (value == NativeDracoDataType::DT_INT32) {
      return bridging::toJs(rt, 5);
    } else if (value == NativeDracoDataType::DT_UINT32) {
      return bridging::toJs(rt, 6);
    } else if (value == NativeDracoDataType::DT_INT64) {
      return bridging::toJs(rt, 7);
    } else if (value == NativeDracoDataType::DT_UINT64) {
      return bridging::toJs(rt, 8);
    } else if (value == NativeDracoDataType::DT_FLOAT32) {
      return bridging::toJs(rt, 9);
    } else if (value == NativeDracoDataType::DT_FLOAT64) {
      return bridging::toJs(rt, 10);
    } else if (value == NativeDracoDataType::DT_BOOL) {
      return bridging::toJs(rt, 11);
    } else if (value == NativeDracoDataType::DT_TYPES_COUNT) {
      return bridging::toJs(rt, 12);
    } else {
      throw jsi::JSError(rt, "No appropriate enum member found for enum value");
    }
  }
};

#pragma mark - NativeDracoEncodedGeometryType

enum class NativeDracoEncodedGeometryType { INVALID_GEOMETRY_TYPE, POINT_CLOUD, TRIANGULAR_MESH, NUM_ENCODED_GEOMETRY_TYPES };

template <>
struct Bridging<NativeDracoEncodedGeometryType> {
  static NativeDracoEncodedGeometryType fromJs(jsi::Runtime &rt, const jsi::Value &rawValue) {
    double value = (double)rawValue.asNumber();
    if (value == -1) {
      return NativeDracoEncodedGeometryType::INVALID_GEOMETRY_TYPE;
    } else if (value == 0) {
      return NativeDracoEncodedGeometryType::POINT_CLOUD;
    } else if (value == 1) {
      return NativeDracoEncodedGeometryType::TRIANGULAR_MESH;
    } else if (value == 2) {
      return NativeDracoEncodedGeometryType::NUM_ENCODED_GEOMETRY_TYPES;
    } else {
      throw jsi::JSError(rt, "No appropriate enum member found for value");
    }
  }

  static jsi::Value toJs(jsi::Runtime &rt, NativeDracoEncodedGeometryType value) {
    if (value == NativeDracoEncodedGeometryType::INVALID_GEOMETRY_TYPE) {
      return bridging::toJs(rt, -1);
    } else if (value == NativeDracoEncodedGeometryType::POINT_CLOUD) {
      return bridging::toJs(rt, 0);
    } else if (value == NativeDracoEncodedGeometryType::TRIANGULAR_MESH) {
      return bridging::toJs(rt, 1);
    } else if (value == NativeDracoEncodedGeometryType::NUM_ENCODED_GEOMETRY_TYPES) {
      return bridging::toJs(rt, 2);
    } else {
      throw jsi::JSError(rt, "No appropriate enum member found for enum value");
    }
  }
};

#pragma mark - NativeDracoGeometryAttribute

enum class NativeDracoGeometryAttribute { INVALID, POSITION, NORMAL, COLOR, TEX_COORD, GENERIC, TANGENT, MATERIAL, JOINTS, WEIGHTS, NAMED_ATTRIBUTES_COUNT };

template <>
struct Bridging<NativeDracoGeometryAttribute> {
  static NativeDracoGeometryAttribute fromJs(jsi::Runtime &rt, const jsi::Value &rawValue) {
    double value = (double)rawValue.asNumber();
    if (value == -1) {
      return NativeDracoGeometryAttribute::INVALID;
    } else if (value == 0) {
      return NativeDracoGeometryAttribute::POSITION;
    } else if (value == 1) {
      return NativeDracoGeometryAttribute::NORMAL;
    } else if (value == 2) {
      return NativeDracoGeometryAttribute::COLOR;
    } else if (value == 3) {
      return NativeDracoGeometryAttribute::TEX_COORD;
    } else if (value == 4) {
      return NativeDracoGeometryAttribute::GENERIC;
    } else if (value == 5) {
      return NativeDracoGeometryAttribute::TANGENT;
    } else if (value == 6) {
      return NativeDracoGeometryAttribute::MATERIAL;
    } else if (value == 7) {
      return NativeDracoGeometryAttribute::JOINTS;
    } else if (value == 8) {
      return NativeDracoGeometryAttribute::WEIGHTS;
    } else if (value == 9) {
      return NativeDracoGeometryAttribute::NAMED_ATTRIBUTES_COUNT;
    } else {
      throw jsi::JSError(rt, "No appropriate enum member found for value");
    }
  }

  static jsi::Value toJs(jsi::Runtime &rt, NativeDracoGeometryAttribute value) {
    if (value == NativeDracoGeometryAttribute::INVALID) {
      return bridging::toJs(rt, -1);
    } else if (value == NativeDracoGeometryAttribute::POSITION) {
      return bridging::toJs(rt, 0);
    } else if (value == NativeDracoGeometryAttribute::NORMAL) {
      return bridging::toJs(rt, 1);
    } else if (value == NativeDracoGeometryAttribute::COLOR) {
      return bridging::toJs(rt, 2);
    } else if (value == NativeDracoGeometryAttribute::TEX_COORD) {
      return bridging::toJs(rt, 3);
    } else if (value == NativeDracoGeometryAttribute::GENERIC) {
      return bridging::toJs(rt, 4);
    } else if (value == NativeDracoGeometryAttribute::TANGENT) {
      return bridging::toJs(rt, 5);
    } else if (value == NativeDracoGeometryAttribute::MATERIAL) {
      return bridging::toJs(rt, 6);
    } else if (value == NativeDracoGeometryAttribute::JOINTS) {
      return bridging::toJs(rt, 7);
    } else if (value == NativeDracoGeometryAttribute::WEIGHTS) {
      return bridging::toJs(rt, 8);
    } else if (value == NativeDracoGeometryAttribute::NAMED_ATTRIBUTES_COUNT) {
      return bridging::toJs(rt, 9);
    } else {
      throw jsi::JSError(rt, "No appropriate enum member found for enum value");
    }
  }
};

#pragma mark - NativeDracoStatus

enum class NativeDracoStatus { OK, DRACO_ERROR, IO_ERROR, INVALID_PARAMETER, UNSUPPORTED_VERSION, UNKNOWN_VERSION, UNSUPPORTED_FEATURE };

template <>
struct Bridging<NativeDracoStatus> {
  static NativeDracoStatus fromJs(jsi::Runtime &rt, const jsi::Value &rawValue) {
    double value = (double)rawValue.asNumber();
    if (value == 0) {
      return NativeDracoStatus::OK;
    } else if (value == -1) {
      return NativeDracoStatus::DRACO_ERROR;
    } else if (value == -2) {
      return NativeDracoStatus::IO_ERROR;
    } else if (value == -3) {
      return NativeDracoStatus::INVALID_PARAMETER;
    } else if (value == -4) {
      return NativeDracoStatus::UNSUPPORTED_VERSION;
    } else if (value == -5) {
      return NativeDracoStatus::UNKNOWN_VERSION;
    } else if (value == -6) {
      return NativeDracoStatus::UNSUPPORTED_FEATURE;
    } else {
      throw jsi::JSError(rt, "No appropriate enum member found for value");
    }
  }

  static jsi::Value toJs(jsi::Runtime &rt, NativeDracoStatus value) {
    if (value == NativeDracoStatus::OK) {
      return bridging::toJs(rt, 0);
    } else if (value == NativeDracoStatus::DRACO_ERROR) {
      return bridging::toJs(rt, -1);
    } else if (value == NativeDracoStatus::IO_ERROR) {
      return bridging::toJs(rt, -2);
    } else if (value == NativeDracoStatus::INVALID_PARAMETER) {
      return bridging::toJs(rt, -3);
    } else if (value == NativeDracoStatus::UNSUPPORTED_VERSION) {
      return bridging::toJs(rt, -4);
    } else if (value == NativeDracoStatus::UNKNOWN_VERSION) {
      return bridging::toJs(rt, -5);
    } else if (value == NativeDracoStatus::UNSUPPORTED_FEATURE) {
      return bridging::toJs(rt, -6);
    } else {
      throw jsi::JSError(rt, "No appropriate enum member found for enum value");
    }
  }
};
  class JSI_EXPORT NativeDracoCxxSpecJSI : public TurboModule {
protected:
  NativeDracoCxxSpecJSI(std::shared_ptr<CallInvoker> jsInvoker);

public:
  virtual jsi::Object createDecoderModule(jsi::Runtime &rt) = 0;
  virtual jsi::Object createEncoderModule(jsi::Runtime &rt) = 0;
  virtual void attachPointCloudHandle(jsi::Runtime &rt, jsi::Object handle) = 0;
  virtual void attachMeshHandle(jsi::Runtime &rt, jsi::Object handle) = 0;
  virtual void attachPointAttributeHandle(jsi::Runtime &rt, jsi::Object handle) = 0;
  virtual void installMeshMethods(jsi::Runtime &rt, jsi::Object handle) = 0;
  virtual void installPointCloudMethods(jsi::Runtime &rt, jsi::Object handle) = 0;
  virtual void installPointAttributeMethods(jsi::Runtime &rt, jsi::Object handle) = 0;
  virtual void initBuffer(jsi::Runtime &rt, jsi::Object bufferHandle, jsi::Object data, int length) = 0;
  virtual jsi::Value GetEncodedGeometryType_Deprecated(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object inBuffer) = 0;
  virtual bool GetAttributeDataArrayForAllPoints(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object pointCloudHandle, jsi::Object pointAttributeHandle, jsi::Value dataType, int outSize, jsi::Object outValues) = 0;
  virtual void SkipAttributeTransform(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Value attributeType) = 0;
  virtual jsi::Object GetAttributeByUniqueId(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object pointCloudHandle, int uniqueId) = 0;
  virtual int GetAttributeId(jsi::Runtime &rt, jsi::Object pointCloudHandle, jsi::Value attributeType) = 0;
  virtual jsi::Object GetAttribute(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object pointCloudHandle, int attributeId) = 0;
  virtual bool GetTrianglesUInt32Array(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object meshHandle, int outSize, jsi::Object outValues) = 0;
  virtual jsi::Value DecodeBufferToPointCloud(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object bufferHandle, jsi::Object pointCloudHandle) = 0;
  virtual jsi::Value DecodeBufferToMesh(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object bufferHandle, jsi::Object meshHandle) = 0;

};

template <typename T>
class JSI_EXPORT NativeDracoCxxSpec : public TurboModule {
public:
  jsi::Value get(jsi::Runtime &rt, const jsi::PropNameID &propName) override {
    return delegate_.get(rt, propName);
  }

  static constexpr std::string_view kModuleName = "Draco";

protected:
  NativeDracoCxxSpec(std::shared_ptr<CallInvoker> jsInvoker)
    : TurboModule(std::string{NativeDracoCxxSpec::kModuleName}, jsInvoker),
      delegate_(reinterpret_cast<T*>(this), jsInvoker) {}


private:
  class Delegate : public NativeDracoCxxSpecJSI {
  public:
    Delegate(T *instance, std::shared_ptr<CallInvoker> jsInvoker) :
      NativeDracoCxxSpecJSI(std::move(jsInvoker)), instance_(instance) {

    }

    jsi::Object createDecoderModule(jsi::Runtime &rt) override {
      static_assert(
          bridging::getParameterCount(&T::createDecoderModule) == 1,
          "Expected createDecoderModule(...) to have 1 parameters");

      return bridging::callFromJs<jsi::Object>(
          rt, &T::createDecoderModule, jsInvoker_, instance_);
    }
    jsi::Object createEncoderModule(jsi::Runtime &rt) override {
      static_assert(
          bridging::getParameterCount(&T::createEncoderModule) == 1,
          "Expected createEncoderModule(...) to have 1 parameters");

      return bridging::callFromJs<jsi::Object>(
          rt, &T::createEncoderModule, jsInvoker_, instance_);
    }
    void attachPointCloudHandle(jsi::Runtime &rt, jsi::Object handle) override {
      static_assert(
          bridging::getParameterCount(&T::attachPointCloudHandle) == 2,
          "Expected attachPointCloudHandle(...) to have 2 parameters");

      return bridging::callFromJs<void>(
          rt, &T::attachPointCloudHandle, jsInvoker_, instance_, std::move(handle));
    }
    void attachMeshHandle(jsi::Runtime &rt, jsi::Object handle) override {
      static_assert(
          bridging::getParameterCount(&T::attachMeshHandle) == 2,
          "Expected attachMeshHandle(...) to have 2 parameters");

      return bridging::callFromJs<void>(
          rt, &T::attachMeshHandle, jsInvoker_, instance_, std::move(handle));
    }
    void attachPointAttributeHandle(jsi::Runtime &rt, jsi::Object handle) override {
      static_assert(
          bridging::getParameterCount(&T::attachPointAttributeHandle) == 2,
          "Expected attachPointAttributeHandle(...) to have 2 parameters");

      return bridging::callFromJs<void>(
          rt, &T::attachPointAttributeHandle, jsInvoker_, instance_, std::move(handle));
    }
    void installMeshMethods(jsi::Runtime &rt, jsi::Object handle) override {
      static_assert(
          bridging::getParameterCount(&T::installMeshMethods) == 2,
          "Expected installMeshMethods(...) to have 2 parameters");

      return bridging::callFromJs<void>(
          rt, &T::installMeshMethods, jsInvoker_, instance_, std::move(handle));
    }
    void installPointCloudMethods(jsi::Runtime &rt, jsi::Object handle) override {
      static_assert(
          bridging::getParameterCount(&T::installPointCloudMethods) == 2,
          "Expected installPointCloudMethods(...) to have 2 parameters");

      return bridging::callFromJs<void>(
          rt, &T::installPointCloudMethods, jsInvoker_, instance_, std::move(handle));
    }
    void installPointAttributeMethods(jsi::Runtime &rt, jsi::Object handle) override {
      static_assert(
          bridging::getParameterCount(&T::installPointAttributeMethods) == 2,
          "Expected installPointAttributeMethods(...) to have 2 parameters");

      return bridging::callFromJs<void>(
          rt, &T::installPointAttributeMethods, jsInvoker_, instance_, std::move(handle));
    }
    void initBuffer(jsi::Runtime &rt, jsi::Object bufferHandle, jsi::Object data, int length) override {
      static_assert(
          bridging::getParameterCount(&T::initBuffer) == 4,
          "Expected initBuffer(...) to have 4 parameters");

      return bridging::callFromJs<void>(
          rt, &T::initBuffer, jsInvoker_, instance_, std::move(bufferHandle), std::move(data), std::move(length));
    }
    jsi::Value GetEncodedGeometryType_Deprecated(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object inBuffer) override {
      static_assert(
          bridging::getParameterCount(&T::GetEncodedGeometryType_Deprecated) == 3,
          "Expected GetEncodedGeometryType_Deprecated(...) to have 3 parameters");

      return bridging::callFromJs<jsi::Value>(
          rt, &T::GetEncodedGeometryType_Deprecated, jsInvoker_, instance_, std::move(decoderHandle), std::move(inBuffer));
    }
    bool GetAttributeDataArrayForAllPoints(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object pointCloudHandle, jsi::Object pointAttributeHandle, jsi::Value dataType, int outSize, jsi::Object outValues) override {
      static_assert(
          bridging::getParameterCount(&T::GetAttributeDataArrayForAllPoints) == 7,
          "Expected GetAttributeDataArrayForAllPoints(...) to have 7 parameters");

      return bridging::callFromJs<bool>(
          rt, &T::GetAttributeDataArrayForAllPoints, jsInvoker_, instance_, std::move(decoderHandle), std::move(pointCloudHandle), std::move(pointAttributeHandle), std::move(dataType), std::move(outSize), std::move(outValues));
    }
    void SkipAttributeTransform(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Value attributeType) override {
      static_assert(
          bridging::getParameterCount(&T::SkipAttributeTransform) == 3,
          "Expected SkipAttributeTransform(...) to have 3 parameters");

      return bridging::callFromJs<void>(
          rt, &T::SkipAttributeTransform, jsInvoker_, instance_, std::move(decoderHandle), std::move(attributeType));
    }
    jsi::Object GetAttributeByUniqueId(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object pointCloudHandle, int uniqueId) override {
      static_assert(
          bridging::getParameterCount(&T::GetAttributeByUniqueId) == 4,
          "Expected GetAttributeByUniqueId(...) to have 4 parameters");

      return bridging::callFromJs<jsi::Object>(
          rt, &T::GetAttributeByUniqueId, jsInvoker_, instance_, std::move(decoderHandle), std::move(pointCloudHandle), std::move(uniqueId));
    }
    int GetAttributeId(jsi::Runtime &rt, jsi::Object pointCloudHandle, jsi::Value attributeType) override {
      static_assert(
          bridging::getParameterCount(&T::GetAttributeId) == 3,
          "Expected GetAttributeId(...) to have 3 parameters");

      return bridging::callFromJs<int>(
          rt, &T::GetAttributeId, jsInvoker_, instance_, std::move(pointCloudHandle), std::move(attributeType));
    }
    jsi::Object GetAttribute(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object pointCloudHandle, int attributeId) override {
      static_assert(
          bridging::getParameterCount(&T::GetAttribute) == 4,
          "Expected GetAttribute(...) to have 4 parameters");

      return bridging::callFromJs<jsi::Object>(
          rt, &T::GetAttribute, jsInvoker_, instance_, std::move(decoderHandle), std::move(pointCloudHandle), std::move(attributeId));
    }
    bool GetTrianglesUInt32Array(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object meshHandle, int outSize, jsi::Object outValues) override {
      static_assert(
          bridging::getParameterCount(&T::GetTrianglesUInt32Array) == 5,
          "Expected GetTrianglesUInt32Array(...) to have 5 parameters");

      return bridging::callFromJs<bool>(
          rt, &T::GetTrianglesUInt32Array, jsInvoker_, instance_, std::move(decoderHandle), std::move(meshHandle), std::move(outSize), std::move(outValues));
    }
    jsi::Value DecodeBufferToPointCloud(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object bufferHandle, jsi::Object pointCloudHandle) override {
      static_assert(
          bridging::getParameterCount(&T::DecodeBufferToPointCloud) == 4,
          "Expected DecodeBufferToPointCloud(...) to have 4 parameters");

      return bridging::callFromJs<jsi::Value>(
          rt, &T::DecodeBufferToPointCloud, jsInvoker_, instance_, std::move(decoderHandle), std::move(bufferHandle), std::move(pointCloudHandle));
    }
    jsi::Value DecodeBufferToMesh(jsi::Runtime &rt, jsi::Object decoderHandle, jsi::Object bufferHandle, jsi::Object meshHandle) override {
      static_assert(
          bridging::getParameterCount(&T::DecodeBufferToMesh) == 4,
          "Expected DecodeBufferToMesh(...) to have 4 parameters");

      return bridging::callFromJs<jsi::Value>(
          rt, &T::DecodeBufferToMesh, jsInvoker_, instance_, std::move(decoderHandle), std::move(bufferHandle), std::move(meshHandle));
    }

  private:
    friend class NativeDracoCxxSpec;
    T *instance_;
  };

  Delegate delegate_;
};

} // namespace facebook::react