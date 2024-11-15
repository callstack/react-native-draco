import { StyleSheet, View, Text, Button } from 'react-native';
import {
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

export default function App() {
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
