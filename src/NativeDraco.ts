import type { TurboModule } from 'react-native';
import { TurboModuleRegistry } from 'react-native';
import type {
  Int32,
  UnsafeObject,
} from 'react-native/Libraries/Types/CodegenTypes';

export enum EncodedGeometryType {
  INVALID_GEOMETRY_TYPE = -1,
  POINT_CLOUD = 0,
  TRIANGULAR_MESH = 1,
  NUM_ENCODED_GEOMETRY_TYPES = 2,
}

export enum DataType {
  // Not a legal value for DataType. Used to indicate a field has not been set.
  DT_INVALID = 0,
  DT_INT8 = 1,
  DT_UINT8 = 2,
  DT_INT16 = 3,
  DT_UINT16 = 4,
  DT_INT32 = 5,
  DT_UINT32 = 6,
  DT_INT64 = 7,
  DT_UINT64 = 8,
  DT_FLOAT32 = 9,
  DT_FLOAT64 = 10,
  DT_BOOL = 11,
  DT_TYPES_COUNT = 12,
}

export enum GeometryAttribute {
  INVALID = -1,
  // Named attributes start here. The difference between named and generic
  // attributes is that for named attributes we know their purpose and we
  // can apply some special methods when dealing with them (e.g. during
  // encoding).
  POSITION = 0,
  NORMAL = 1,
  COLOR = 2,
  TEX_COORD = 3,
  // A special id used to mark attributes that are not assigned to any known
  // predefined use case. Such attributes are often used for a shader specific
  // data.
  GENERIC = 4,
  TANGENT = 5,
  MATERIAL = 6,
  JOINTS = 7,
  WEIGHTS = 8,
  NAMED_ATTRIBUTES_COUNT = 9,
}

export enum Status {
  OK = 0,
  DRACO_ERROR = -1, // Used for general errors.
  IO_ERROR = -2, // Error when handling input or output stream.
  INVALID_PARAMETER = -3, // Invalid parameter passed to a function.
  UNSUPPORTED_VERSION = -4, // Input not compatible with the current version.
  UNKNOWN_VERSION = -5, // Input was created with an unknown version of
  // the library.
  UNSUPPORTED_FEATURE = -6, // Input contains feature that is not supported.
}

export type OpaqueNativeDecoderHandle = UnsafeObject;
export type OpaqueNativeBufferHandle = UnsafeObject;
export type OpaqueNativePointCloudHandle = UnsafeObject;
export type OpaqueNativePointAttributeHandle = UnsafeObject;
export type OpaqueNativeMeshHandle = UnsafeObject;

export interface Spec extends TurboModule {
  // Native module methods
  createDecoderModule(): OpaqueNativeDecoderHandle;
  createEncoderModule(): OpaqueNativeDecoderHandle;

  attachPointCloudHandle(handle: OpaqueNativePointCloudHandle): void;
  attachMeshHandle(handle: OpaqueNativeMeshHandle): void;
  attachPointAttributeHandle(handle: OpaqueNativePointAttributeHandle): void;

  installMeshMethods(handle: OpaqueNativeMeshHandle): void;
  installPointCloudMethods(handle: OpaqueNativePointCloudHandle): void;
  installPointAttributeMethods(handle: OpaqueNativePointAttributeHandle): void;

  // Buffer operations
  initBuffer(
    bufferHandle: OpaqueNativeBufferHandle,
    data: UnsafeObject,
    length: Int32
  ): void;

  // Decoder
  GetEncodedGeometryType_Deprecated(
    decoderHandle: OpaqueNativeDecoderHandle,
    inBuffer: OpaqueNativeBufferHandle
  ): EncodedGeometryType;

  GetAttributeDataArrayForAllPoints(
    decoderHandle: OpaqueNativeDecoderHandle,
    pointCloudHandle: OpaqueNativePointCloudHandle,
    pointAttributeHandle: OpaqueNativePointAttributeHandle,
    dataType: DataType,
    outSize: Int32,
    outValues: UnsafeObject
  ): boolean;

  SkipAttributeTransform(
    decoderHandle: OpaqueNativeDecoderHandle,
    attributeType: GeometryAttribute
  ): void;

  GetAttributeByUniqueId(
    decoderHandle: OpaqueNativeDecoderHandle,
    pointCloudHandle: OpaqueNativePointCloudHandle,
    uniqueId: Int32
  ): OpaqueNativePointAttributeHandle;

  GetAttributeId(
    pointCloudHandle: OpaqueNativePointCloudHandle,
    attributeType: GeometryAttribute
  ): Int32;

  GetAttribute(
    decoderHandle: OpaqueNativeDecoderHandle,
    pointCloudHandle: OpaqueNativePointCloudHandle,
    attributeId: Int32
  ): OpaqueNativePointAttributeHandle;

  GetTrianglesUInt32Array(
    decoderHandle: OpaqueNativeDecoderHandle,
    meshHandle: OpaqueNativeMeshHandle,
    outSize: Int32,
    outValues: UnsafeObject
  ): boolean;

  DecodeBufferToPointCloud(
    decoderHandle: OpaqueNativeDecoderHandle,
    bufferHandle: OpaqueNativeBufferHandle,
    pointCloudHandle: OpaqueNativePointCloudHandle
  ): Status;

  // Decode methods
  DecodeBufferToMesh(
    decoderHandle: OpaqueNativeDecoderHandle,
    bufferHandle: OpaqueNativeBufferHandle,
    meshHandle: OpaqueNativeMeshHandle
  ): Status;
}

export default TurboModuleRegistry.getEnforcing<Spec>('Draco');
