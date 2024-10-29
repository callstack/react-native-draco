import { StyleSheet, View, Text, Button } from 'react-native';
import { DracoDecoderModule } from 'react-native-draco';

export default function App() {
  const decode = () => {
    const byteArray = new Uint8Array(0);
    // Create the Draco decoder.
    const decoderModule = DracoDecoderModule();
    const buffer = new decoderModule.DecoderBuffer();
    buffer.Init(byteArray, byteArray.length);

    // Create a buffer to hold the encoded data.
    const decoder = new decoderModule.Decoder();
    const geometryType = decoder.GetEncodedGeometryType(buffer);
    console.log({ geometryType });

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

    console.log(status);

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
