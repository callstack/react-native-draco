#include "draco/core/decoder_buffer.h"
#pragma once

#include <jsi/jsi.h>

#include "draco/attributes/attribute_transform_type.h"
#include "draco/attributes/point_attribute.h"
#include "draco/compression/config/compression_shared.h"
#include "draco/compression/decode.h"
#include "draco/core/decoder_buffer.h"
#include "draco/mesh/mesh.h"

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

template <class T>
static bool GetAttributeDataArrayForAllPointsHelper(const draco::PointCloud &pc,
                                                    const draco::PointAttribute &pa,
                                                    const draco::DataType type,
                                                    int out_size,
                                                    void *out_values) {
  const int components = pa.num_components();
  const int num_points = pc.num_points();
  const size_t data_size = num_points * components * sizeof(T);
  if (data_size != out_size) {
    return false;
  }
  const bool requested_type_matches = pa.data_type() == type;
  if (requested_type_matches && pa.is_mapping_identity()) {
    // Copy values directly to the output vector.
    const uint8_t *ptr = pa.GetAddress(draco::AttributeValueIndex(0));
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
}
