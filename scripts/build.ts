const { $, cd } = require('zx');

const PLATFORMS = ['arm64-ios', 'x86_64-ios', 'arm64-iossimulator'];

const build = async (platform: string) => {
  await $`cmake ../ -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/${platform}.cmake && make`;
};

const main = async () => {
  await cd(`${__dirname}/../externals/draco`);
  // Build for each platform
  for (const platform of PLATFORMS) {
    console.log(`Building for ${platform}`);
    await $`mkdir -p build-${platform}`;
    await cd(`build-${platform}`);
    await build(platform);
    cd(`..`);
    console.log(`Build for ${platform} done`);
  }
  // Combine simulator libraries
  await $`mkdir -p build-simulator-ios`;
  await $`lipo -create build-arm64-iossimulator/libdraco.a build-x86_64-ios/libdraco.a -output build-simulator-ios/libdraco.a`;

  // Create XCFramework
  await $`xcodebuild -create-xcframework -library build-arm64-ios/libdraco.a -library build-simulator-ios/libdraco.a -output libdraco.xcframework`;
  // Copy to output
  await $`cp -r libdraco.xcframework ../../libs/`;
};

main();
