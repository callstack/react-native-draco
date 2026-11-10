// Host tests for attribute extraction and native-state lookup
// (callstack/react-native-draco#5). Build + run: bash cpp/tests/run.sh
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "draco-helpers.h"
#include "draco-state.h"
#include "draco/compression/encode.h"
#include "draco/mesh/triangle_soup_mesh_builder.h"

using namespace facebook::react;
namespace jsi = facebook::jsi;

static int failures = 0;
static int checks = 0;

#define CHECK(name, cond)                                    \
  do {                                                       \
    ++checks;                                                \
    if (!(cond)) {                                           \
      ++failures;                                            \
      std::printf("FAIL: %s  (%s:%d)\n", std::string(name).c_str(), \
                  __FILE__, __LINE__);                       \
    }                                                        \
  } while (0)

// ---------------------------------------------------------------------------
// Source data: a quad (4 vertices, 2 triangles) with skinning-style attributes.
// ---------------------------------------------------------------------------
static const float kPos[4][3] = {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}};
static const uint8_t kJointsU8[4][4] = {
    {0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11}, {12, 13, 14, 255}};
static const uint16_t kJointsU16[4][4] = {
    {0, 300, 1, 2}, {300, 2, 3, 4}, {1000, 5, 6, 7}, {65535, 8, 9, 10}};
static const float kWeights[4][4] = {
    {.5f, .5f, 0, 0}, {.7f, .3f, 0, 0}, {1, 0, 0, 0}, {.25f, .75f, 0, 0}};
static const int8_t kI8[4][4] = {
    {-1, 2, -3, 4}, {-128, 127, 0, -5}, {6, -7, 8, -9}, {10, -11, 12, -13}};
static const int16_t kI16[4][4] = {{-1000, 2, -3, 4},
                                   {-32768, 32767, 0, -5},
                                   {600, -700, 8, -9},
                                   {10, -11, 12, -13}};
static const int kTris[2][3] = {{0, 1, 2}, {0, 2, 3}};

// Attribute order as added to the builder (preserved through encode/decode).
enum AttrIdx { POS = 0, J8, J16, W, I8, I16, ATTR_COUNT };

static std::unique_ptr<draco::Mesh> BuildQuad() {
  draco::TriangleSoupMeshBuilder b;
  b.Start(2);
  const int pos = b.AddAttribute(draco::GeometryAttribute::POSITION, 3, draco::DT_FLOAT32);
  const int j8 = b.AddAttribute(draco::GeometryAttribute::GENERIC, 4, draco::DT_UINT8);
  const int j16 = b.AddAttribute(draco::GeometryAttribute::GENERIC, 4, draco::DT_UINT16);
  const int w = b.AddAttribute(draco::GeometryAttribute::GENERIC, 4, draco::DT_FLOAT32);
  const int i8 = b.AddAttribute(draco::GeometryAttribute::GENERIC, 4, draco::DT_INT8);
  const int i16 = b.AddAttribute(draco::GeometryAttribute::GENERIC, 4, draco::DT_INT16);
  for (int f = 0; f < 2; ++f) {
    const int *i = kTris[f];
    const draco::FaceIndex fi(f);
    b.SetAttributeValuesForFace(pos, fi, kPos[i[0]], kPos[i[1]], kPos[i[2]]);
    b.SetAttributeValuesForFace(j8, fi, kJointsU8[i[0]], kJointsU8[i[1]], kJointsU8[i[2]]);
    b.SetAttributeValuesForFace(j16, fi, kJointsU16[i[0]], kJointsU16[i[1]], kJointsU16[i[2]]);
    b.SetAttributeValuesForFace(w, fi, kWeights[i[0]], kWeights[i[1]], kWeights[i[2]]);
    b.SetAttributeValuesForFace(i8, fi, kI8[i[0]], kI8[i[1]], kI8[i[2]]);
    b.SetAttributeValuesForFace(i16, fi, kI16[i[0]], kI16[i[1]], kI16[i[2]]);
  }
  return b.Finalize();
}

// Encodes |src| with |method| and decodes the result into |out|.
static bool EncodeDecodeInto(const draco::Mesh &src, int method, draco::Mesh *out) {
  draco::Encoder enc;
  enc.SetEncodingMethod(method);
  // Quantize POSITION and float GENERIC (WEIGHTS); integer GENERIC stays lossless.
  enc.SetAttributeQuantization(draco::GeometryAttribute::POSITION, 14);
  enc.SetAttributeQuantization(draco::GeometryAttribute::GENERIC, 8);
  draco::EncoderBuffer eb;
  if (!enc.EncodeMeshToBuffer(src, &eb).ok()) return false;
  draco::DecoderBuffer db;
  db.Init(eb.data(), eb.size());
  draco::Decoder dec;
  return dec.DecodeBufferToGeometry(&db, out).ok();
}

// For each decoded point, the index of the source vertex it came from, found by
// matching the decoded POSITION to the nearest source position.
static std::vector<int> SourceVertexPerPoint(const draco::Mesh &m) {
  const draco::PointAttribute &pa = *m.attribute(POS);
  std::vector<int> out(m.num_points(), -1);
  for (draco::PointIndex p(0); p < m.num_points(); ++p) {
    float v[3];
    pa.GetValue(pa.mapped_index(p), v);
    int best = -1;
    float bestD = 1e9f;
    for (int s = 0; s < 4; ++s) {
      const float dx = v[0] - kPos[s][0], dy = v[1] - kPos[s][1], dz = v[2] - kPos[s][2];
      const float d = dx * dx + dy * dy + dz * dz;
      if (d < bestD) { bestD = d; best = s; }
    }
    out[p.value()] = best;
  }
  return out;
}

// Expected per-point output: source vertex values laid out point by point.
template <class Out, class In, int C>
static std::vector<Out> Expected(const std::vector<int> &srcOf, const In (&src)[4][C]) {
  std::vector<Out> out;
  for (int s : srcOf)
    for (int c = 0; c < C; ++c) out.push_back(static_cast<Out>(src[s][c]));
  return out;
}

template <class T>
static bool Extract(const draco::Mesh &m, int attr, draco::DataType type, std::vector<T> *out) {
  const draco::PointAttribute &pa = *m.attribute(attr);
  out->assign(m.num_points() * pa.num_components(), T(0));
  return GetAttributeDataArrayForAllPointsByType(m, pa, type,
                                                 static_cast<int>(out->size() * sizeof(T)),
                                                 out->data());
}

template <class T>
static bool SameInts(const std::vector<T> &a, const std::vector<T> &b) {
  return a == b;
}

static bool NearFloats(const std::vector<float> &a, const std::vector<float> &b, float tol) {
  if (a.size() != b.size()) return false;
  for (size_t i = 0; i < a.size(); ++i)
    if (std::fabs(a[i] - b[i]) > tol) return false;
  return true;
}

// ---------------------------------------------------------------------------
// Attribute extraction, per encoding method.
// ---------------------------------------------------------------------------
static void TestExtraction(const draco::Mesh &m, const std::string &method) {
  const std::vector<int> srcOf = SourceVertexPerPoint(m);
  auto name = [&](const char *n) { return method + ": " + n; };

  // Each attribute requested as its own stored type.
  {
    std::vector<float> got;
    const bool ok = Extract(m, POS, draco::DT_FLOAT32, &got);
    CHECK(name("POSITION float32 as FLOAT32 returns values of the source vertices"),
          ok && NearFloats(got, Expected<float>(srcOf, kPos), 0.01f));
  }
  {
    std::vector<uint8_t> got;
    const bool ok = Extract(m, J8, draco::DT_UINT8, &got);
    CHECK(name("JOINTS uint8 as UINT8 returns values of the source vertices"),
          ok && SameInts(got, Expected<uint8_t>(srcOf, kJointsU8)));
  }
  {
    std::vector<uint16_t> got;
    const bool ok = Extract(m, J16, draco::DT_UINT16, &got);
    CHECK(name("JOINTS uint16 as UINT16 keeps values above 255"),
          ok && SameInts(got, Expected<uint16_t>(srcOf, kJointsU16)));
  }
  {
    std::vector<float> got;
    const bool ok = Extract(m, W, draco::DT_FLOAT32, &got);
    CHECK(name("WEIGHTS float32 as FLOAT32 returns values of the source vertices"),
          ok && NearFloats(got, Expected<float>(srcOf, kWeights), 0.01f));
  }
  {
    std::vector<int8_t> got;
    const bool ok = Extract(m, I8, draco::DT_INT8, &got);
    CHECK(name("int8 as INT8 keeps negative values"),
          ok && SameInts(got, Expected<int8_t>(srcOf, kI8)));
  }
  {
    std::vector<int16_t> got;
    const bool ok = Extract(m, I16, draco::DT_INT16, &got);
    CHECK(name("int16 as INT16 keeps negative values"),
          ok && SameInts(got, Expected<int16_t>(srcOf, kI16)));
  }

  // Conversion to a different requested type.
  {
    std::vector<uint16_t> got;
    const bool ok = Extract(m, J8, draco::DT_UINT16, &got);
    CHECK(name("JOINTS uint8 as UINT16 returns widened values"),
          ok && SameInts(got, Expected<uint16_t>(srcOf, kJointsU8)));
  }
  {
    std::vector<uint32_t> got;
    const bool ok = Extract(m, J8, draco::DT_UINT32, &got);
    CHECK(name("JOINTS uint8 as UINT32 returns widened values"),
          ok && SameInts(got, Expected<uint32_t>(srcOf, kJointsU8)));
  }
  {
    std::vector<float> got;
    const bool ok = Extract(m, J8, draco::DT_FLOAT32, &got);
    CHECK(name("JOINTS uint8 as FLOAT32 returns float values"),
          ok && NearFloats(got, Expected<float>(srcOf, kJointsU8), 0.0f));
  }

  // Output size that does not match points * components * sizeof(T).
  {
    const draco::PointAttribute &pa = *m.attribute(J8);
    const int exact = m.num_points() * pa.num_components() * 1;
    std::vector<uint8_t> buf(exact + 1);
    CHECK(name("UINT8 request with buffer one element short returns false"),
          !GetAttributeDataArrayForAllPointsByType(m, pa, draco::DT_UINT8, exact - 1, buf.data()));
    CHECK(name("UINT8 request with buffer one element long returns false"),
          !GetAttributeDataArrayForAllPointsByType(m, pa, draco::DT_UINT8, exact + 1, buf.data()));
  }
  {
    const draco::PointAttribute &pa = *m.attribute(POS);
    const int exact = m.num_points() * pa.num_components() * 4;
    std::vector<uint8_t> buf(exact + 4);
    CHECK(name("FLOAT32 request with buffer one element short returns false"),
          !GetAttributeDataArrayForAllPointsByType(m, pa, draco::DT_FLOAT32, exact - 4, buf.data()));
  }

  // Unsupported requested types.
  {
    const draco::PointAttribute &pa = *m.attribute(J8);
    const int bytes8 = m.num_points() * pa.num_components() * 8;
    std::vector<uint8_t> buf(bytes8);
    CHECK(name("DT_INVALID request returns false"),
          !GetAttributeDataArrayForAllPointsByType(m, pa, draco::DT_INVALID, bytes8, buf.data()));
    CHECK(name("DT_INT64 request returns false"),
          !GetAttributeDataArrayForAllPointsByType(m, pa, draco::DT_INT64, bytes8, buf.data()));
    CHECK(name("DT_FLOAT64 request returns false"),
          !GetAttributeDataArrayForAllPointsByType(m, pa, draco::DT_FLOAT64, bytes8, buf.data()));
  }

  // Template helper directly, type derived from T.
  {
    const draco::PointAttribute &pa = *m.attribute(J8);
    std::vector<uint8_t> got(m.num_points() * pa.num_components());
    const bool ok = GetAttributeDataArrayForAllPointsHelper<uint8_t>(
        m, pa, static_cast<int>(got.size()), got.data());
    CHECK(name("Helper<uint8_t> on JOINTS uint8 returns values of the source vertices"),
          ok && SameInts(got, Expected<uint8_t>(srcOf, kJointsU8)));
  }
}

// ---------------------------------------------------------------------------
// Native state lookup.
// ---------------------------------------------------------------------------
template <class Fn>
static bool ThrowsJSError(Fn fn) {
  try {
    fn();
  } catch (const jsi::JSError &) {
    return true;
  } catch (...) {
    return false;
  }
  return false;
}

// Stores fn()'s result in |out|; returns false if fn throws.
template <class Fn, class R>
static bool ReturnsWithoutThrow(Fn fn, R *out) {
  try {
    *out = fn();
    return true;
  } catch (...) {
    return false;
  }
}

static void TestRequireDracoObject() {
  jsi::Runtime rt;
  {
    jsi::Object empty(rt);
    CHECK("requireDracoObject throws JSError when object has no native state",
          ThrowsJSError([&] { requireDracoObject<DracoMesh>(rt, empty, "mesh"); }));
  }
  {
    jsi::Object obj(rt);
    obj.setNativeState(rt, std::make_shared<DracoDecoder>());
    CHECK("requireDracoObject throws JSError when native state is another type",
          ThrowsJSError([&] { requireDracoObject<DracoMesh>(rt, obj, "mesh"); }));
  }
  {
    jsi::Object obj(rt);
    auto mesh = std::make_shared<DracoMesh>();
    obj.setNativeState(rt, mesh);
    std::shared_ptr<DracoMesh> got;
    const bool ok = ReturnsWithoutThrow(
        [&] { return requireDracoObject<DracoMesh>(rt, obj, "mesh"); }, &got);
    CHECK("requireDracoObject returns the stored state when type matches",
          ok && got && got.get() == mesh.get());
  }
}

static void TestRequirePointCloud(const draco::Mesh &src) {
  jsi::Runtime rt;
  {
    jsi::Object obj(rt);
    auto pc = std::make_shared<DracoPointCloud>();
    obj.setNativeState(rt, pc);
    std::shared_ptr<draco::PointCloud> got;
    const bool ok = ReturnsWithoutThrow([&] { return requirePointCloud(rt, obj); }, &got);
    CHECK("requirePointCloud accepts DracoPointCloud state",
          ok && got && got.get() == &pc->pointCloud_);
  }
  {
    jsi::Object obj(rt);
    auto mesh = std::make_shared<DracoMesh>();
    const bool decoded =
        EncodeDecodeInto(src, draco::MESH_EDGEBREAKER_ENCODING, &mesh->mesh_);
    const uint32_t expectedPoints = mesh->mesh_.num_points();
    obj.setNativeState(rt, mesh);
    std::shared_ptr<draco::PointCloud> got;
    const bool ok = ReturnsWithoutThrow([&] { return requirePointCloud(rt, obj); }, &got);
    CHECK("requirePointCloud accepts DracoMesh state with the mesh's points",
          decoded && expectedPoints > 0 && ok && got &&
              got.get() == &mesh->mesh_ && got->num_points() == expectedPoints);
  }
  {
    jsi::Object obj(rt);
    obj.setNativeState(rt, std::make_shared<DracoDecoder>());
    CHECK("requirePointCloud throws JSError for DracoDecoder state",
          ThrowsJSError([&] { requirePointCloud(rt, obj); }));
  }
  {
    jsi::Object empty(rt);
    CHECK("requirePointCloud throws JSError when object has no native state",
          ThrowsJSError([&] { requirePointCloud(rt, empty); }));
  }
}

int main() {
  auto src = BuildQuad();
  if (!src) {
    std::puts("setup: building quad mesh failed");
    return 1;
  }
  struct Method { int id; const char *name; } methods[] = {
      {draco::MESH_EDGEBREAKER_ENCODING, "edgebreaker"},
      {draco::MESH_SEQUENTIAL_ENCODING, "sequential"}};
  for (const Method &mt : methods) {
    draco::Mesh m;
    if (!EncodeDecodeInto(*src, mt.id, &m) || m.num_attributes() != ATTR_COUNT) {
      std::printf("setup: %s encode/decode failed\n", mt.name);
      return 1;
    }
    std::printf("%s: points=%u identity_mapping(JOINTS u8)=%d\n", mt.name, m.num_points(),
                m.attribute(J8)->is_mapping_identity());
    TestExtraction(m, mt.name);
  }
  TestRequireDracoObject();
  TestRequirePointCloud(*src);

  std::printf("%d/%d checks failed\n", failures, checks);
  return failures ? 1 : 0;
}
