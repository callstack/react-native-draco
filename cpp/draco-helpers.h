#pragma once

#include <cstring>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include <jsi/jsi.h>

#include "draco/attributes/attribute_transform_type.h"
#include "draco/attributes/point_attribute.h"
#include "draco/compression/config/compression_shared.h"
#include "draco/compression/decode.h"
#include "draco/core/decoder_buffer.h"
#include "draco/mesh/mesh.h"
#include "draco-state.h"

#define HOSTFN(name, size, capture) \
jsi::Function::createFromHostFunction(rt, jsi::PropNameID::forAscii(rt, name), size, \
capture(jsi::Runtime &rt, const jsi::Value &thisValue, \
const jsi::Value *arguments, size_t count)          \
->jsi::Value

namespace facebook::react {
template <typename T>
std::shared_ptr<T> tryGetDracoObject(jsi::Runtime& rt, jsi::Object& obj) {
  if (!obj.hasNativeState(rt)) {
    return nullptr;
  }
  
  return std::dynamic_pointer_cast<T>(obj.getNativeState(rt));
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

namespace detail {
template <class T>
constexpr draco::DataType dracoDataTypeOf() {
  if constexpr (std::is_same_v<T, int8_t>) return draco::DT_INT8;
  else if constexpr (std::is_same_v<T, uint8_t>) return draco::DT_UINT8;
  else if constexpr (std::is_same_v<T, int16_t>) return draco::DT_INT16;
  else if constexpr (std::is_same_v<T, uint16_t>) return draco::DT_UINT16;
  else if constexpr (std::is_same_v<T, int32_t>) return draco::DT_INT32;
  else if constexpr (std::is_same_v<T, uint32_t>) return draco::DT_UINT32;
  else if constexpr (std::is_same_v<T, float>) return draco::DT_FLOAT32;
  else static_assert(sizeof(T) == 0, "Unsupported attribute element type");
}
}  // namespace detail

// Mirrors Draco's Emscripten wrapper (GetAttributeDataArrayForAllPoints).
template <class T>
bool GetAttributeDataArrayForAllPointsHelper(const draco::PointCloud &pc,
                                             const draco::PointAttribute &pa,
                                             int out_size,
                                             void *out_values) {
  const int components = pa.num_components();
  const size_t data_size = static_cast<size_t>(pc.num_points()) *
                           static_cast<size_t>(components) * sizeof(T);
  if (out_size < 0 || data_size != static_cast<size_t>(out_size)) {
    return false;
  }
  const bool requested_type_matches = pa.data_type() == detail::dracoDataTypeOf<T>();
  if (requested_type_matches && pa.is_mapping_identity()) {
    if (data_size > 0) {
      const uint8_t *ptr = pa.GetAddress(draco::AttributeValueIndex(0));
      ::memcpy(out_values, ptr, data_size);
    }
    return true;
  }

  std::vector<T> values(components);
  size_t entry_id = 0;
  T *const out = reinterpret_cast<T *>(out_values);
  for (draco::PointIndex i(0); i < pc.num_points(); ++i) {
    const draco::AttributeValueIndex val_index = pa.mapped_index(i);
    if (requested_type_matches) {
      pa.GetValue(val_index, values.data());
    } else if (!pa.ConvertValue<T>(val_index, values.data())) {
      return false;
    }
    for (int j = 0; j < components; ++j) {
      out[entry_id++] = values[j];
    }
  }
  return true;
}

inline bool GetAttributeDataArrayForAllPointsByType(const draco::PointCloud &pc,
                                                    const draco::PointAttribute &pa,
                                                    draco::DataType type,
                                                    int out_size,
                                                    void *out_values) {
  switch (type) {
    case draco::DT_INT8:
      return GetAttributeDataArrayForAllPointsHelper<int8_t>(pc, pa, out_size, out_values);
    case draco::DT_UINT8:
      return GetAttributeDataArrayForAllPointsHelper<uint8_t>(pc, pa, out_size, out_values);
    case draco::DT_INT16:
      return GetAttributeDataArrayForAllPointsHelper<int16_t>(pc, pa, out_size, out_values);
    case draco::DT_UINT16:
      return GetAttributeDataArrayForAllPointsHelper<uint16_t>(pc, pa, out_size, out_values);
    case draco::DT_INT32:
      return GetAttributeDataArrayForAllPointsHelper<int32_t>(pc, pa, out_size, out_values);
    case draco::DT_UINT32:
      return GetAttributeDataArrayForAllPointsHelper<uint32_t>(pc, pa, out_size, out_values);
    case draco::DT_FLOAT32:
      return GetAttributeDataArrayForAllPointsHelper<float>(pc, pa, out_size, out_values);
    default:
      return false;
  }
}

// Throws a JS error when the handle has no native state of type T.
template <class T>
std::shared_ptr<T> requireDracoObject(jsi::Runtime &rt, jsi::Object &obj, const char *what) {
  auto ptr = tryGetDracoObject<T>(rt, obj);
  if (ptr == nullptr) {
    throw jsi::JSError(rt, std::string("Invalid handle: expected ") + what);
  }
  return ptr;
}

// Accepts a DracoPointCloud or DracoMesh handle; the result keeps the owner alive.
inline std::shared_ptr<draco::PointCloud> requirePointCloud(jsi::Runtime &rt, jsi::Object &obj) {
  if (auto pc = tryGetDracoObject<DracoPointCloud>(rt, obj)) {
    return std::shared_ptr<draco::PointCloud>(pc, &pc->pointCloud_);
  }
  if (auto mesh = tryGetDracoObject<DracoMesh>(rt, obj)) {
    return std::shared_ptr<draco::PointCloud>(mesh, &mesh->mesh_);
  }
  throw jsi::JSError(rt, "Invalid handle: expected point cloud or mesh");
}
}
