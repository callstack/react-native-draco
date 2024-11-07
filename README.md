# react-native-draco

React Native bindings for [Draco](https://github.com/google/draco) - a library for compressing and decompressing 3D geometric meshes and point clouds.

## Installation

```sh
yarn add @callstack/react-native-draco
```

## Usage

For documentation on how to use the Draco library, please refer to the [official documentation](https://google.github.io/draco/).

```js
const decoderModule = DracoDecoderModule();
const buffer = new decoderModule.DecoderBuffer();
buffer.Init(byteArray, byteArray.length);
```


## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT

---

Made with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)
