#include "QuadFrameMeshBuilder.h"

using namespace std;
using namespace PJ;

QuadFrameMeshBuilder::QuadFrameMeshBuilder(Vector2 worldSize, Vector2 strokeSize) :
    worldSize(worldSize),
    strokeSize(strokeSize) {}

Mesh QuadFrameMeshBuilder::BuildMesh() {
    GUARDR(worldSize.x > 0 && worldSize.y > 0, {})

    Mesh result;
    auto frameSize = Vector2(
        std::abs(worldSize.x - strokeSize.x * 2.0f), std::abs(worldSize.y - strokeSize.y * 2.0f)
    );
    auto frameRatioX = strokeSize.x / worldSize.x;
    auto frameRatioY = strokeSize.y / worldSize.y;

    VectorList<Vector3> vertices{
        // Outer frame
        { worldSize.x / 2.0f * vecLeft, worldSize.y * 0.5f * vecUp, 0 },
        { worldSize.x / 2.0f * vecRight, worldSize.y * 0.5f * vecUp, 0 },
        { worldSize.x / 2.0f * vecRight, worldSize.y * 0.5f * vecDown, 0 },
        { worldSize.x / 2.0f * vecLeft, worldSize.y * 0.5f * vecDown, 0 },

        // Inner frame
        { frameSize.x / 2.0f * vecLeft, frameSize.y * 0.5f * vecUp, 0 },
        { frameSize.x / 2.0f * vecRight, frameSize.y * 0.5f * vecUp, 0 },
        { frameSize.x / 2.0f * vecRight, frameSize.y * 0.5f * vecDown, 0 },
        { frameSize.x / 2.0f * vecLeft, frameSize.y * 0.5f * vecDown, 0 }
    };

    VectorList<Vector2> uvs{ // Outer frame
                             { 0, 1 },
                             { 1, 1 },
                             { 1, 0 },
                             { 0, 0 },

                             // Inner frame
                             { frameRatioX, 1.0f - frameRatioY },
                             { 1.0f - frameRatioX, 1.0f - frameRatioY },
                             { 1.0f - frameRatioX, frameRatioY },
                             { frameRatioX, frameRatioY }
    };

    auto meshVertexCount = MeshVertexCount();
    auto trianglesSize = meshVertexCount * 3;
    VectorList<uint32_t> triangles(trianglesSize, 0);

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
