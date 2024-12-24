#include "CenterPolyFrameMeshBuilder.h"
#include "CenterPolyMeshBuilder.h"

using namespace std;
using namespace PJ;

CenterPolyFrameMeshBuilder::CenterPolyFrameMeshBuilder(Config config) :
    worldSize(config.worldSize),
    strokeWidth(config.strokeWidth),
    model(config.model),
    startCap(config.startCap),
    endCap(config.endCap) {}

Mesh CenterPolyFrameMeshBuilder::BuildMesh() {
    GUARDR(worldSize.x > 0 && worldSize.y > 0, {})

    Mesh result;

    auto poly = CenterPolyBuilder().Build({}, worldSize, model);
    GUARDR(poly.Count() >= 2, {});

    Vector2 strokeInset = Vector2{ strokeWidth, strokeWidth } * 2.0f;
    auto innerPoly = CenterPolyBuilder().Build({}, worldSize - strokeInset, model);

    bool isClosed = model.IsClosed();

    auto segmentCount = isClosed ? poly.Count() : poly.Count() - 1;
    auto vertexCount = (poly.Count()) * 2;
    GUARDR(segmentCount > 0 && vertexCount > 0, {})

    VectorList<Vector3> vertices(vertexCount);
    auto uvCount = vertexCount;
    VectorList<Vector2> uvs(uvCount);
    auto trianglesCount = segmentCount * 6;
    VectorList<uint32_t> triangles(trianglesCount);

    auto polygonMin = poly.Min();
    auto polygonSize = poly.Size();

    // Outer vertices
    int vi = 0;
    for (auto& vertex : poly.Value()) {
        vertices[vi] = vertex;
        uvs[vi] = Vector2(
            (vertex.x - polygonMin.x) / polygonSize.x, (vertex.y - polygonMin.y) / polygonSize.y
        );
        vi++;
    }

    // Inner vertices
    for (auto& vertex : innerPoly.Value()) {
        vertices[vi] = vertex;
        uvs[vi] = Vector2(
            (vertex.x - polygonMin.x) / polygonSize.x, (vertex.y - polygonMin.y) / polygonSize.y
        );
        vi++;
    }

    size_t offset = 0;
    for (size_t i = 0; i < segmentCount; i++) {
        bool isClosing = isClosed && i == segmentCount - 1;

        triangles[offset] = (uint32_t)i;
        triangles[offset + 1] = (uint32_t)i + 1;
        triangles[offset + 2] = (uint32_t)(i + poly.Count());

        triangles[offset + 3] = (uint32_t)i + 1;
        triangles[offset + 4] = (uint32_t)(i + poly.Count() + 1);
        triangles[offset + 5] = (uint32_t)(i + poly.Count());

        if (isClosing) {
            triangles[offset + 1] = 0;
            triangles[offset + 2] = (uint32_t)(i + poly.Count());
            triangles[offset + 3] = 0;
            triangles[offset + 4] = (uint32_t)poly.Count();
            triangles[offset + 5] = triangles[offset + 2];
        }

        offset += 6;
    }

    result.Update(vertices, triangles, uvs);

    if (!isClosed) {
        Vector2 strokeSize{ strokeWidth, strokeWidth };

        // FUTURE optimize end caps with partial arc mesh instead of circle (requires angle
        // calculations)
        if (PathCap::Round == startCap) {
            CenterPolyMeshBuilder capBuilder(strokeSize, CenterPolyModel::ellipse);
            auto capMesh = capBuilder.BuildMesh();
            Vector2 midPoint = Lerp(vertices[triangles[0]], vertices[triangles[2]], 0.5f);
            capMesh.Offset(midPoint);

            result += capMesh;
        }

        if (PathCap::Round == endCap) {
            CenterPolyMeshBuilder capBuilder(strokeSize, CenterPolyModel::ellipse);
            auto capMesh = capBuilder.BuildMesh();
            Vector2 midPoint = Lerp(
                vertices[triangles[triangles.size() - 3]],
                vertices[triangles[triangles.size() - 2]], 0.5f
            );
            capMesh.Offset(midPoint);

            result += capMesh;
        }
    }

    return result;
}
