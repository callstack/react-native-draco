// Writes a small Draco-compressed "skinned" quad used by the example app's smoke test.
// Attribute order (GetAttribute index):
//   0 POSITION  float32 x3
//   1 JOINTS    uint8   x4  (GENERIC)
//   2 JOINTS    uint16  x4  (GENERIC, includes values > 255)
//   3 WEIGHTS   float32 x4  (GENERIC, quantized)
// Edgebreaker encoding, so the decoded attributes use a non-identity point mapping.
//
// Usage: make_skinned_fixture <out.drc>
#include <cstdio>
#include <fstream>

#include "draco/compression/encode.h"
#include "draco/mesh/triangle_soup_mesh_builder.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::fprintf(stderr, "usage: %s <out.drc>\n", argv[0]);
    return 2;
  }
  draco::TriangleSoupMeshBuilder b;
  b.Start(2);
  const int pos = b.AddAttribute(draco::GeometryAttribute::POSITION, 3, draco::DT_FLOAT32);
  const int j8 = b.AddAttribute(draco::GeometryAttribute::GENERIC, 4, draco::DT_UINT8);
  const int j16 = b.AddAttribute(draco::GeometryAttribute::GENERIC, 4, draco::DT_UINT16);
  const int w = b.AddAttribute(draco::GeometryAttribute::GENERIC, 4, draco::DT_FLOAT32);
  const float p[4][3] = {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}};
  const uint8_t ja[4][4] = {{0, 1, 0, 0}, {1, 2, 0, 0}, {2, 3, 0, 0}, {3, 255, 0, 0}};
  const uint16_t jb[4][4] = {{0, 300, 0, 0}, {300, 2, 0, 0}, {2, 1000, 0, 0}, {65535, 0, 0, 0}};
  const float wa[4][4] = {{.5f, .5f, 0, 0}, {.7f, .3f, 0, 0}, {1, 0, 0, 0}, {.25f, .75f, 0, 0}};
  const int tri[2][3] = {{0, 1, 2}, {0, 2, 3}};
  for (int f = 0; f < 2; ++f) {
    const int *i = tri[f];
    const draco::FaceIndex fi(f);
    b.SetAttributeValuesForFace(pos, fi, p[i[0]], p[i[1]], p[i[2]]);
    b.SetAttributeValuesForFace(j8, fi, ja[i[0]], ja[i[1]], ja[i[2]]);
    b.SetAttributeValuesForFace(j16, fi, jb[i[0]], jb[i[1]], jb[i[2]]);
    b.SetAttributeValuesForFace(w, fi, wa[i[0]], wa[i[1]], wa[i[2]]);
  }
  auto mesh = b.Finalize();

  draco::Encoder enc;
  enc.SetEncodingMethod(draco::MESH_EDGEBREAKER_ENCODING);
  enc.SetAttributeQuantization(draco::GeometryAttribute::POSITION, 14);
  enc.SetAttributeQuantization(draco::GeometryAttribute::GENERIC, 8);
  draco::EncoderBuffer eb;
  if (!enc.EncodeMeshToBuffer(*mesh, &eb).ok()) {
    std::fprintf(stderr, "encode failed\n");
    return 1;
  }
  std::ofstream out(argv[1], std::ios::binary);
  out.write(eb.data(), eb.size());
  std::printf("wrote %zu bytes to %s\n", eb.size(), argv[1]);
  return out ? 0 : 1;
}
