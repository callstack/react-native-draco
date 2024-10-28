const { $, cd } = require('zx');

const copy_header_files = async (
  source_dir: string,
  destination_dir: string
) => {
  // Create destination directory if it doesn't exist
  await $`mkdir -p ${destination_dir}`;

  // Find all .h files in source directory and its subdirectories
  const find_result = await $`find ${source_dir} -name "*.h"`;
  const header_files = find_result.stdout.trim().split('\n');

  for (const source_file of header_files) {
    if (!source_file) continue;

    // Get the relative path from source directory
    const relative_path = source_file.replace(source_dir, '');
    const dest_file = `${destination_dir}${relative_path}`;

    // Create destination subdirectories if they don't exist
    await $`mkdir -p $(dirname ${dest_file})`;

    // Copy the file
    await $`cp -p ${source_file} ${dest_file}`;
    console.log(`Copied: ${source_file} -> ${dest_file}`);
  }
};

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

  const source_dir = './src';
  const header_destination = '../../libs/include';
  await copy_header_files(source_dir, header_destination);
  $`cp ./build-arm64-ios/draco/draco_features.h ../../libs/include/draco/draco_features.h`;
};

main();
