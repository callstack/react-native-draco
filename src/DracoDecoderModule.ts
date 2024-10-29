import NativeDraco, {
  Status,
  type OpaqueNativeDecoderHandle,
} from './NativeDraco';

class DecoderBuffer {
  bufferArray: Uint8Array | null = null;

  Init(array: Uint8Array, _: number) {
    this.bufferArray = array;
    NativeDraco.initBuffer(this, array.buffer, array.length);
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
    pointCloudHandle: PointCloud,
    uniqueId: number
  ): PointAttribute {
    return NativeDraco.GetAttributeByUniqueId(
      this.decoderHandle,
      pointCloudHandle,
      uniqueId
    ) as PointAttribute;
  }

  GetAttributeId(pointCloudHandle: PointCloud, attributeType: number): number {
    return NativeDraco.GetAttributeId(
      this.decoderHandle,
      pointCloudHandle,
      attributeType
    );
  }

  GetAttribute(
    pointCloudHandle: PointCloud,
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
  size(): number {
    return 0;
  }

  // From GeometryAttribute
  attribute_type(): number {
    return 0;
  }

  data_type(): number {
    return 0;
  }

  num_components(): number {
    return 0;
  }

  normalized(): boolean {
    return false;
  }

  byte_stride(): number {
    return 0;
  }

  byte_offset(): number {
    return 0;
  }

  unique_id(): number {
    return 0;
  }
}

class Mesh {
  // meshHandle: OpaqueNativeDracoHandle;

  constructor() {
    // this.meshHandle = NativeDraco.createMesh();
  }

  num_points(): number {
    return 0;
  }

  num_attributes(): number {
    return 0;
  }

  num_faces(): number {
    return 0;
  }
}

class PointCloud {
  constructor() {
    NativeDraco.attachPointCloudHandle(this);
  }

  num_points(): number {
    return 0;
  }

  num_attributes(): number {
    return 0;
  }
}

export function DracoDecoderModule() {
  return {
    // Constants
    TRIANGULAR_MESH: 1,
    POINT_CLOUD: 0,
    INVALID_GEOMETRY: -1,

    DecoderBuffer,
    Decoder,
    Mesh,
    PointCloud,
    destroy: (module: DecoderBuffer | Decoder | Mesh | PointCloud) => {
      console.log('destroying', module);
    },
  };
}
