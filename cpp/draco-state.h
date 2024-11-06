#pragma once

#include "draco/core/decoder_buffer.h"
#include <jsi/jsi.h>

#include "draco/attributes/point_attribute.h"
#include "draco/core/decoder_buffer.h"
#include "draco/mesh/mesh.h"

namespace facebook::react {
class DracoDecoder: public jsi::NativeState {
public:
  DracoDecoder(): decoder_(draco::Decoder()), last_status_(draco::Status()) {}
  virtual ~DracoDecoder() {}
  
  draco::Decoder decoder_;
  draco::Status last_status_;
};

class DracoDecoderBuffer : public jsi::NativeState {
public:
  DracoDecoderBuffer() : buffer_(draco::DecoderBuffer()) {}
  virtual ~DracoDecoderBuffer() {}
  
  draco::DecoderBuffer buffer_;
};

class DracoPointCloud: public jsi::NativeState {
public:
  DracoPointCloud(): pointCloud_(draco::PointCloud()) {}
  virtual ~DracoPointCloud() {}
  
  draco::PointCloud pointCloud_;
};

class DracoPointAttribute: public jsi::NativeState {
public:
  DracoPointAttribute(): pointAttribute_(draco::PointAttribute()) {}
  virtual ~DracoPointAttribute() {}
  
  explicit DracoPointAttribute(const draco::PointAttribute &attribute): pointAttribute_(draco::PointAttribute()) {
    pointAttribute_.CopyFrom(attribute);
  }
  
  explicit DracoPointAttribute(draco::PointAttribute &&attribute): pointAttribute_(std::move(attribute)) {}
  
  draco::PointAttribute pointAttribute_;
};

class DracoMesh: public jsi::NativeState {
public:
  DracoMesh(): mesh_(draco::Mesh()) {}
  virtual ~DracoMesh() {}
  
  draco::Mesh mesh_;
};
}
