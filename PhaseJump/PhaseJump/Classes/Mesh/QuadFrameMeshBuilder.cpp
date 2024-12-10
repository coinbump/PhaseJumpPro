#include "QuadFrameMeshBuilder.h"

using namespace std;
using namespace PJ;

QuadFrameMeshBuilder::QuadFrameMeshBuilder(Vector2 worldSize, Vector2 strokeSize) :
    worldSize(worldSize),
    strokeSize(strokeSize) {}

Mesh QuadFrameMeshBuilder::BuildMesh() {
    Mesh result;

    auto meshVertexCount = MeshVertexCount();
    VectorList<Vector3> vertices(meshVertexCount, Vector3{});
    auto trianglesSize = meshVertexCount * 3;
    VectorList<uint32_t> triangles(trianglesSize, 0);
    auto uvSize = meshVertexCount;
    VectorList<Vector2> uvs(uvSize, Vector2{});

    // Outer frame
    vertices[0] = Vector3(-worldSize.x / 2.0f, worldSize.y * 0.5f * vecUp, 0);
    vertices[1] = Vector3(worldSize.x / 2.0f, worldSize.y * 0.5f * vecUp, 0);
    vertices[2] = Vector3(worldSize.x / 2.0f, worldSize.y * 0.5f * vecDown, 0);
    vertices[3] = Vector3(-worldSize.x / 2.0f, worldSize.y * 0.5f * vecDown, 0);
    uvs[0] = Vector2(0, 1);
    uvs[1] = Vector2(1, 1);
    uvs[2] = Vector2(1, 0);
    uvs[3] = Vector2(0, 0);

    // Inner frame
    auto frameSize = Vector2(
        std::abs(worldSize.x - strokeSize.x * 2.0f), std::abs(worldSize.y - strokeSize.y * 2.0f)
    );
    vertices[4] = Vector3(-frameSize.x / 2.0f, frameSize.y * 0.5f * vecUp, 0);
    vertices[5] = Vector3(frameSize.x / 2.0f, frameSize.y * 0.5f * vecUp, 0);
    vertices[6] = Vector3(frameSize.x / 2.0f, frameSize.y * 0.5f * vecDown, 0);
    vertices[7] = Vector3(-frameSize.x / 2.0f, frameSize.y * 0.5f * vecDown, 0);

    auto frameRatioX = strokeSize.x / worldSize.x;
    auto frameRatioY = strokeSize.y / worldSize.y;
    uvs[4] = Vector2(frameRatioX, 1.0f - frameRatioY);
    uvs[5] = Vector2(1.0f - frameRatioX, 1.0f - frameRatioY);
    uvs[6] = Vector2(1.0f - frameRatioX, frameRatioY);
    uvs[7] = Vector2(frameRatioX, frameRatioY);

    auto t = 0;
    auto polyVertexCount = PolyVertexCount();

    for (int i = 0; i < polyVertexCount; i++) {
        auto nextInnerVertexIndex =
            ((i + polyVertexCount + 1) - polyVertexCount) % polyVertexCount + polyVertexCount;

        triangles[t] = i;
        triangles[t + 1] = (uint32_t)nextInnerVertexIndex;
        triangles[t + 2] = (uint32_t)(i + polyVertexCount);
        triangles[t + 3] = i;

        triangles[t + 4] = (i + 1) % polyVertexCount; // Wrap to first vertex on last triangle
        triangles[t + 5] = (uint32_t)nextInnerVertexIndex;

        t += 6;
    }

    result.Update(vertices, triangles, uvs);

    return result;
}
