#include "PolyFrameMeshBuilder.h"

using namespace std;
using namespace PJ;

Mesh PolyFrameMeshBuilder::BuildMesh() {
    Mesh mesh;
    GUARDR(poly.size() >= 2, mesh);

    auto segmentCount = SegmentCount();

    auto vertexCount = MeshVertexCount();
    VectorList<Vector3> vertices(vertexCount, Vector3::zero);
    auto trianglesSize = segmentCount * 6;
    VectorList<uint32_t> triangles(trianglesSize, 0);
    auto uvSize = vertexCount;
    VectorList<Vector2> uvs(uvSize, Vector2::zero);

    // Outer frame
    auto meshVertexIndex = 0;
    for (size_t i = 0; i < segmentCount; i++) {
        auto vertex = poly[i];
        auto nextVertex = poly[(i + 1) % poly.size()];

        Vector2 angleVector(nextVertex.x - vertex.x, nextVertex.y - vertex.y);
        // Debug.Log("Angle Vector: " + angleVector.ToString());
        Angle degreeAngle(angleVector);
        // Debug.Log("Degree Angle: " + degreeAngle.ToString());

        auto outerOffset =
            Angle::DegreesAngle(degreeAngle.Degrees() - 90.0f).ToVector2(strokeWidth / 2.0f);
        auto innerOffset =
            Angle::DegreesAngle(degreeAngle.Degrees() + 90.0f).ToVector2(strokeWidth / 2.0f);

        // Debug.Log("Outer Offset: " + outerOffset.ToString());
        // Debug.Log("Inner Offset: " + innerOffset.ToString());

        // Outer frame
        // FUTURE: support 3D lines (requires cylinder)
        vertices[meshVertexIndex + 0] = vertex + Vector3(innerOffset.x, innerOffset.y, 0);
        vertices[meshVertexIndex + 1] = nextVertex + Vector3(innerOffset.x, innerOffset.y, 0);
        vertices[meshVertexIndex + 2] = vertex + Vector3(outerOffset.x, outerOffset.y, 0);
        vertices[meshVertexIndex + 3] = nextVertex + Vector3(outerOffset.x, outerOffset.y, 0);

        meshVertexIndex += 4;
    }

    auto center = poly.Center();
    auto size = poly.Size();
    if (size.x > 0 && size.y > 0) {
        for (size_t i = 0; i < uvSize; i++) {
            auto vertex = vertices[i];
            auto distanceFromCenter = vertex - center;
            Vector2 ratio(
                (distanceFromCenter.x + size.x / 2.0f) / size.x,
                (distanceFromCenter.y + size.y / 2.0f) / size.y
            );
            uvs[i] = ratio;
        }
    } else {
        for (size_t i = 0; i < uvSize; i++) {
            uvs[i] = Vector2::zero;
        }
    }

    auto t = 0;
    meshVertexIndex = 0;
    for (size_t i = 0; i < segmentCount; i++) {
        triangles[t] = meshVertexIndex;
        triangles[t + 1] = meshVertexIndex + 2;
        triangles[t + 2] = meshVertexIndex + 1;
        triangles[t + 3] = meshVertexIndex + 2;
        triangles[t + 4] = meshVertexIndex + 3;
        triangles[t + 5] = meshVertexIndex + 1;

        meshVertexIndex += 4;
        t += 6;
    }

    mesh.Update(vertices, triangles, uvs);
    return mesh;
}
