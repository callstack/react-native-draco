import { StyleSheet, View, Text, Button } from 'react-native';
import {
  DracoDecoderModule,
  EncodedGeometryType,
  Status,
} from 'react-native-draco';
import RNFetchBlob from 'react-native-blob-util';

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

    console.log({ status: Status[status], outputGeometry });

    if ('num_faces' in outputGeometry) {
      const num_faces = outputGeometry.num_faces();
      console.log({ num_faces });
    }
    const num_points = outputGeometry.num_points();

    console.log({ num_points });

    // You must explicitly delete objects created from the DracoDecoderModule
    // or Decoder.
    decoderModule.destroy(outputGeometry);
    decoderModule.destroy(decoder);
    decoderModule.destroy(buffer);
  };

  return (
    <View style={styles.container}>
      <Text>Draco Example</Text>
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
