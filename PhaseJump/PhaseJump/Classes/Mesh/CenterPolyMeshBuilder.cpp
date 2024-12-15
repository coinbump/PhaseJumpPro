#include "CenterPolyMeshBuilder.h"
#include "CenterPolyBuilder.h"

using namespace std;
using namespace PJ;

Mesh CenterPolyMeshBuilder::BuildMesh() {
    GUARDR(worldSize.x > 0 && worldSize.y > 0, {})

    Mesh result;

    auto poly = CenterPolyBuilder().Build({}, worldSize, model);
    GUARDR(poly.Count() >= 2, {})

    auto vertexCount = poly.Count() + 1;

    VectorList<Vector3> vertices(vertexCount);
    VectorList<Vector2> uvs(vertexCount);

    bool isClosed = model.IsClosed();
    auto segmentCount = isClosed ? poly.Count() : poly.Count() - 1;
    GUARDR(vertexCount > 0 && segmentCount > 0, {})

    auto trianglesCount = segmentCount * 3;

    VectorList<uint32_t> triangles(trianglesCount, 0);

    // Center vertex
    vertices[0] = {};
    uvs[0] = Vector2(0.5f, 0.5f);

    auto polygonMin = poly.Min();
    auto polygonSize = poly.Size();

    // Edge vertices
    int vi = 1;
    for (auto& vertex : poly.Value()) {
        vertices[vi] = vertex;
        uvs[vi] = Vector2(
            (vertex.x - polygonMin.x) / polygonSize.x, (vertex.y - polygonMin.y) / polygonSize.y
        );
        vi++;
    }

    auto offset = 0;
    for (int i = 0; i < segmentCount; i++) {
        bool isClosing = isClosed && i == segmentCount - 1;

        triangles[offset] = 0;
        triangles[offset + 1] = i + 1;
        triangles[offset + 2] = i + 2;

        if (isClosing) {
            triangles[offset + 2] = 1;
        }

        offset += 3;
    }

    result.Update(vertices, triangles, uvs);

    return result;
}
