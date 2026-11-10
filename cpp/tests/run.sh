#!/usr/bin/env bash
# Build and run the host C++ tests for cpp/draco-helpers.h.
#
# Builds a host copy of Draco from externals/draco (needs cmake) on first run,
# unless DRACO_BUILD_DIR already contains libdraco.a and the generated
# draco/draco_features.h.
#
#   cpp/tests/run.sh                    # build + run tests
#   cpp/tests/run.sh --fixture out.drc  # also regenerate the example's skinned.drc
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
DRACO_BUILD_DIR="${DRACO_BUILD_DIR:-$ROOT/build/draco-host}"  # build/ is gitignored
OUT_DIR="${OUT_DIR:-$ROOT/build/cpp-tests}"
CXX="${CXX:-clang++}"
CMAKE="${CMAKE:-cmake}"

if [[ ! -f "$ROOT/externals/draco/src/draco/core/draco_types.h" ]]; then
  echo "externals/draco is missing; run: git submodule update --init externals/draco" >&2
  exit 2
fi

if [[ ! -f "$DRACO_BUILD_DIR/libdraco.a" ]]; then
  echo "Building host Draco in $DRACO_BUILD_DIR"
  "$CMAKE" -S "$ROOT/externals/draco" -B "$DRACO_BUILD_DIR" \
    -DCMAKE_BUILD_TYPE=Release -DCMAKE_POLICY_VERSION_MINIMUM=3.5 > /dev/null
  "$CMAKE" --build "$DRACO_BUILD_DIR" --target draco_static -j 4 > /dev/null
fi

mkdir -p "$OUT_DIR"
cd "$ROOT"

INCLUDES=(-I cpp/tests/stub -I cpp -I externals/draco/src -I "$DRACO_BUILD_DIR")

"$CXX" -std=c++17 -Wall "${INCLUDES[@]}" \
  cpp/tests/attribute_extraction_test.cc \
  "$DRACO_BUILD_DIR/libdraco.a" \
  -o "$OUT_DIR/attribute_extraction_test"

"$OUT_DIR/attribute_extraction_test"

if [[ "${1:-}" == "--fixture" ]]; then
  "$CXX" -std=c++17 -Wall "${INCLUDES[@]}" \
    cpp/tests/make_skinned_fixture.cc \
    "$DRACO_BUILD_DIR/libdraco.a" \
    -o "$OUT_DIR/make_skinned_fixture"
  "$OUT_DIR/make_skinned_fixture" "${2:?usage: run.sh --fixture <out.drc>}"
fi
