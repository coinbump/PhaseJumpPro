#include "GridMeshBuilder.h"

using namespace std;
using namespace PJ;

Mesh GridMeshBuilder::BuildMesh() {
    Mesh mesh;
    GUARDR(gridSize.x > 0 && gridSize.y > 0, mesh)
    GUARDR(worldSize.x > 0 && worldSize.y > 0, mesh)

    int meshVertexCount = MeshVertexCount();
    auto trianglesSize = SegmentCount() * 6;
    auto uvSize = meshVertexCount;

    VectorList<Vector3> vertices(meshVertexCount, Vector3{});
    VectorList<uint32_t> triangles(trianglesSize, 0);
    VectorList<Vector2> uvs(uvSize, Vector2{});

    Vector2 cellSize;
    cellSize.x = worldSize.x / gridSize.x;
    cellSize.y = worldSize.y / gridSize.y;

    Rect viewBounds(
        Vector2((worldSize.x / 2.0f) * vecLeft, (worldSize.y / 2.0f) * vecUp), worldSize
    );

    // Vertical lines
    auto meshVertexIndex = 0;
    auto allLinesWidth = (VerticalLinesCount() - 1) * cellSize.x;
    for (int x = 0; x < VerticalLinesCount(); x++) {
        auto xPosition = (allLinesWidth / 2.0f) * vecLeft + (x * cellSize.x) * vecRight;
        auto xLeft = xPosition + (strokeWidth / 2.0f) * vecLeft;
        auto xRight = xPosition + (strokeWidth / 2.0f) * vecRight;
        auto yTop = (worldSize.y / 2.0f) * vecUp;
        auto yBottom = (worldSize.y / 2.0f) * vecDown;

        vertices[meshVertexIndex + 0] = Vector3(xLeft, yTop, 0);
        vertices[meshVertexIndex + 1] = Vector3(xRight, yTop, 0);
        vertices[meshVertexIndex + 2] = Vector3(xLeft, yBottom, 0);
        vertices[meshVertexIndex + 3] = Vector3(xRight, yBottom, 0);

        meshVertexIndex += 4;
    }

    // Horizontal lines
    auto allLinesHeight = (HorizontalLinesCount() - 1) * cellSize.y;
    for (int y = 0; y < HorizontalLinesCount(); y++) {
        auto yPosition = (allLinesHeight / 2.0f) * vecUp + y * cellSize.y * vecDown;
        auto yTop = yPosition + (strokeWidth / 2.0f) * vecUp;
        auto yBottom = yPosition + (strokeWidth / 2.0f) * vecDown;
        auto xLeft = (worldSize.x / 2.0f) * vecLeft;
        auto xRight = (worldSize.x / 2.0f) * vecRight;

        vertices[meshVertexIndex + 0] = Vector3(xLeft, yTop, 0);
        vertices[meshVertexIndex + 1] = Vector3(xRight, yTop, 0);
        vertices[meshVertexIndex + 2] = Vector3(xLeft, yBottom, 0);
        vertices[meshVertexIndex + 3] = Vector3(xRight, yBottom, 0);

        meshVertexIndex += 4;
    }

    for (int i = 0; i < meshVertexCount; i++) {
        auto meshVertex = vertices[i];
        uvs[i] = Vector2(
            (meshVertex.x - viewBounds.origin.x) / worldSize.x,
            std::abs(meshVertex.y - viewBounds.origin.y) / worldSize.y
        );
    }

    auto t = 0;
    meshVertexIndex = 0;
    for (int i = 0; i < SegmentCount(); i++) {
        triangles[t] = meshVertexIndex;
        triangles[t + 1] = meshVertexIndex + 1;
        triangles[t + 2] = meshVertexIndex + 2;
        triangles[t + 3] = meshVertexIndex + 1;
        triangles[t + 4] = meshVertexIndex + 3;
        triangles[t + 5] = meshVertexIndex + 2;

        meshVertexIndex += 4;
        t += 6;
    }

    mesh.Update(vertices, triangles, uvs);
    return mesh;
}
