import NativeDraco, {
  GeometryAttribute,
  Status,
  type OpaqueNativeDecoderHandle,
} from './NativeDraco';

class DecoderBuffer {
  arrayBuffer: Uint8Array | null = null;

  Init(array: Uint8Array, _: number) {
    this.arrayBuffer = array;
    NativeDraco.initBuffer(this, this.arrayBuffer.buffer, array.length);
  }
}

class Decoder {
  decoderHandle: OpaqueNativeDecoderHandle;
  constructor() {
    this.decoderHandle = NativeDraco.createDecoderModule();
  }

  GetEncodedGeometryType(buffer: DecoderBuffer) {
    return NativeDraco.GetEncodedGeometryType_Deprecated(
      this.decoderHandle,
      buffer
    );
  }

  DecodeBufferToPointCloud(
    buffer: DecoderBuffer,
    pointCloud: PointCloud
  ): Status {
    return NativeDraco.DecodeBufferToPointCloud(
      this.decoderHandle,
      buffer,
      pointCloud
    );
  }

  GetAttributeDataArrayForAllPoints(
    pointCloudHandle: PointCloud,
    pointAttributeHandle: PointAttribute,
    dataType: number,
    outSize: number,
    outValues: Object
  ) {
    return NativeDraco.GetAttributeDataArrayForAllPoints(
      this.decoderHandle,
      pointCloudHandle,
      pointAttributeHandle,
      dataType,
      outSize,
      outValues
    );
  }

  GetAttributeByUniqueId(
    pointCloudHandle: PointCloud | Mesh,
    uniqueId: number
  ): PointAttribute {
    return NativeDraco.GetAttributeByUniqueId(
      this.decoderHandle,
      pointCloudHandle,
      Number(uniqueId)
    ) as PointAttribute;
  }

  GetAttributeId(
    pointCloudHandle: PointCloud | Mesh,
    attributeType: GeometryAttribute
  ): number {
    return NativeDraco.GetAttributeId(pointCloudHandle, attributeType);
  }

  GetAttribute(
    pointCloudHandle: PointCloud | Mesh,
    attributeId: number
  ): PointAttribute {
    return NativeDraco.GetAttribute(
      this.decoderHandle,
      pointCloudHandle,
      attributeId
    ) as PointAttribute;
  }

  DecodeBufferToMesh(buffer: DecoderBuffer, mesh: Mesh): Status {
    return NativeDraco.DecodeBufferToMesh(this.decoderHandle, buffer, mesh);
  }
}

class PointAttribute {
  constructor() {
    NativeDraco.attachPointAttributeHandle(this);
  }
}

class Mesh {
  constructor() {
    NativeDraco.attachMeshHandle(this);
  }
}

class PointCloud {
  constructor() {
    NativeDraco.attachPointCloudHandle(this);
  }
}

// This is defined on prototypes in JSI

interface PointAttribute {
  size: () => number;
  attribute_type: () => number;
  data_type: () => number;
  num_components: () => number;
  normalized: () => boolean;
  byte_stride: () => number;
  byte_offset: () => number;
  unique_id: () => number;
}

interface Mesh {
  num_faces: () => number;
  num_points: () => number;
}

interface PointCloud {
  num_points: () => number;
  num_attributes: () => number;
}

export function DracoDecoderModule() {
  NativeDraco.installMeshMethods(Mesh.prototype);
  NativeDraco.installPointCloudMethods(PointCloud.prototype);
  NativeDraco.installPointAttributeMethods(PointAttribute.prototype);

  return {
    // Constants
    TRIANGULAR_MESH: 1,
    POINT_CLOUD: 0,
    INVALID_GEOMETRY: -1,

    DecoderBuffer,
    Decoder,
    Mesh,
    PointCloud,
    destroy: (_: DecoderBuffer | Decoder | Mesh | PointCloud) => {
      // No-op, objects are automatically cleaned up when they go out of scope.
    },
  };
}
