import { useState } from 'react';
import { StyleSheet, View, Text, Button } from 'react-native';
import {
  DataType,
  DracoDecoderModule,
  EncodedGeometryType,
  GeometryAttribute,
  Status,
} from '@callstack/react-native-draco';
import RNFetchBlob from 'react-native-blob-util';

const defaultAttributeIDs = {
  position: 'POSITION',
  normal: 'NORMAL',
  color: 'COLOR',
  uv: 'TEX_COORD',
};

const defaultAttributeTypes = {
  position: 'Float32Array',
  normal: 'Float32Array',
  color: 'Float32Array',
  uv: 'Float32Array',
};

const readFile = async (path: string): Promise<Uint8Array> => {
  const filePath = RNFetchBlob.fs.asset(path);
  const data = await RNFetchBlob.fs.readFile(filePath, 'base64');
  const byteCharacters = atob(data);
  const byteNumbers = new Array(byteCharacters.length);
  for (let i = 0; i < byteCharacters.length; i++) {
    byteNumbers[i] = byteCharacters.charCodeAt(i);
  }
  return new Uint8Array(byteNumbers);
};

// Smoke test for typed attribute extraction (issue #5). skinned.drc is an
// edgebreaker-encoded quad with JOINTS stored as Uint8 (attribute 1) and
// Uint16 (attribute 2), so the decoded attributes use a non-identity mapping.
// Regenerate with: cpp/tests/run.sh --fixture example/ios/Resources/skinned.drc
const decodeSkinned = async (): Promise<string> => {
  const byteArray = await readFile('skinned.drc');
  const decoderModule = DracoDecoderModule();
  const buffer = new decoderModule.DecoderBuffer();
  buffer.Init(byteArray, byteArray.length);
  const decoder = new decoderModule.Decoder();
  const mesh = new decoderModule.Mesh();
  const failures: string[] = [];
  try {
    const status = decoder.DecodeBufferToMesh(buffer, mesh);
    if (status !== Status.OK) {
      return `FAIL: decode status ${Status[status]}`;
    }
    const numPoints = mesh.num_points();
    const typed = [
      { index: 1, name: 'JOINTS uint8', ArrayType: Uint8Array },
      { index: 2, name: 'JOINTS uint16', ArrayType: Uint16Array },
    ];
    for (const { index, name, ArrayType } of typed) {
      const attribute = decoder.GetAttribute(mesh, index);
      const numValues = numPoints * attribute.num_components();
      const values = new ArrayType(numValues);
      const ok = decoder.GetAttributeDataArrayForAllPoints(
        mesh,
        attribute,
        attribute.data_type(),
        values.byteLength,
        values
      );
      // Reference: the Float32 path, which already handled conversion and mapping.
      const reference = new Float32Array(numValues);
      decoder.GetAttributeDataArrayForAllPoints(
        mesh,
        attribute,
        DataType.DT_FLOAT32,
        reference.byteLength,
        reference
      );
      const same = Array.from(values).every((v, i) => v === reference[i]);
      if (!ok || !same) {
        failures.push(`${name} ok=${ok} values=[${Array.from(values)}]`);
      }
    }
    // An invalid handle must throw a JS error instead of crashing the app.
    let threw = false;
    try {
      decoder.GetAttributeDataArrayForAllPoints(
        {} as any,
        decoder.GetAttribute(mesh, 1),
        DataType.DT_UINT8,
        numPoints * 4,
        new Uint8Array(numPoints * 4)
      );
    } catch {
      threw = true;
    }
    if (!threw) failures.push('invalid handle did not throw');
  } finally {
    decoderModule.destroy(mesh);
    decoderModule.destroy(decoder);
    decoderModule.destroy(buffer);
  }
  return failures.length ? `FAIL: ${failures.join('; ')}` : 'PASS';
};

export default function App() {
  const [skinnedResult, setSkinnedResult] = useState('not run');
  const runSkinned = async () => {
    try {
      setSkinnedResult(await decodeSkinned());
    } catch (e) {
      setSkinnedResult(`FAIL: ${String(e)}`);
    }
  };

  const decode = async () => {
    const byteArray = await readFile('bunny.drc');
    console.log('Draco file loaded!');
    // Create the Draco decoder.
    const decoderModule = DracoDecoderModule();
    const buffer = new decoderModule.DecoderBuffer();
    buffer.Init(byteArray, byteArray.length);

    // Create a buffer to hold the encoded data.
    const decoder = new decoderModule.Decoder();
    const geometryType = decoder.GetEncodedGeometryType(buffer);
    console.log({ geometryType: EncodedGeometryType[geometryType] });

    const t0 = performance.now();
    // Decode the encoded geometry.
    let outputGeometry;
    let status;
    if (geometryType === decoderModule.TRIANGULAR_MESH) {
      outputGeometry = new decoderModule.Mesh();
      status = decoder.DecodeBufferToMesh(buffer, outputGeometry);
    } else {
      outputGeometry = new decoderModule.PointCloud();
      status = decoder.DecodeBufferToPointCloud(buffer, outputGeometry);
    }

    const t1 = performance.now();
    console.log('Draco Decoder finished decoding!');

    console.log(`Decoding took ${(t1 - t0) / 1000} seconds.`);

    console.log({ status: Status[status], outputGeometry });

    if ('num_faces' in outputGeometry) {
      const num_faces = outputGeometry.num_faces();
      console.log({ num_faces });
    }
    const num_points = outputGeometry.num_points();

    console.log({ num_points });

    const useUniqueIDs = true;

    // const geometry = { index: null, attributes: [] };

    for (const attributeName in defaultAttributeIDs) {
      // @ts-ignore
      const attributeType = defaultAttributeTypes[attributeName];

      console.log({ attributeName, attributeType });

      let attribute;
      let attributeID;

      if (useUniqueIDs) {
        // @ts-ignore
        attributeID = defaultAttributeIDs[attributeName];
        attribute = decoder.GetAttributeByUniqueId(outputGeometry, attributeID);
      } else {
        attributeID = decoder.GetAttributeId(
          outputGeometry,
          // @ts-ignore
          GeometryAttribute[defaultAttributeIDs[attributeName]]
        );
        console.log('GetAttributeId', GeometryAttribute[attributeID]);

        if (attributeID === -1) continue;

        attribute = decoder.GetAttribute(outputGeometry, attributeID);
      }

      console.log('Received attribute', {
        attribute_unique_id: attribute.unique_id(),
        attribute_size: attribute.size(),
        attribute_type: attribute.attribute_type(),
        attribute_data_type: attribute.data_type(),
        attribute_num_components: attribute.num_components(),
        attribute_normalized: attribute.normalized(),
        attribute_byte_stride: attribute.byte_stride(),
        attribute_byte_offset: attribute.byte_offset(),
      });

      const numComponents = attribute.num_components();
      const numValues = num_points * numComponents;
      const byteLength = numValues * 4;
      const dataType = attribute.data_type();

      const outputArray = new Float32Array(numValues);
      console.log('GetAttributeDataArrayForAllPoints before:');
      console.log(outputArray.slice(0, 100));

      // NOTE: This API is different from Draco.js api as it relied on wasm memory. Has to be adjusted in IR-Engine.
      decoder.GetAttributeDataArrayForAllPoints(
        outputGeometry,
        attribute,
        dataType,
        byteLength,
        outputArray
      );
      console.log('GetAttributeDataArrayForAllPoints after:');
      console.log(outputArray.slice(0, 100));
    }

    if (
      'num_faces' in outputGeometry &&
      geometryType === decoderModule.TRIANGULAR_MESH
    ) {
      // Generate mesh faces.
      var numFaces = outputGeometry.num_faces();
      var numIndices = numFaces * 3;
      var dataSize = numIndices * 4;
      console.log('GetTrianglesUInt32Array before:');
      // NOTE: This API is different from Draco.js api as it relied on wasm memory. Has to be adjusted in IR-Engine.
      const outputArray = new Uint32Array(dataSize);
      console.log(outputArray.slice(0, 100));
      decoder.GetTrianglesUInt32Array(outputGeometry, dataSize, outputArray);
      console.log('GetTrianglesUInt32Array after:');
      console.log(outputArray.slice(0, 100));
    }

    // You must explicitly delete objects created from the DracoDecoderModule
    // or Decoder.
    decoderModule.destroy(outputGeometry);
    decoderModule.destroy(decoder);
    decoderModule.destroy(buffer);
  };

  return (
    <View style={styles.container}>
      <Text style={{ fontSize: 40 }}>Draco Example</Text>
      <Button title="Decode" onPress={decode} />
      <Button title="Decode skinned" onPress={runSkinned} />
      <Text testID="skinned-result">Skinned: {skinnedResult}</Text>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  box: {
    width: 60,
    height: 60,
    marginVertical: 20,
  },
});
