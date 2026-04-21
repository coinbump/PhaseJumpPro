#include "BoxMeshBuilder.h"
#include "Mesh.h"
#include "VectorList.h"

using namespace std;
using namespace PJ;

BoxMeshBuilder::BoxMeshBuilder(Vector3 worldSize) :
    worldSize(worldSize) {}

Mesh BoxMeshBuilder::BuildMesh() {
    GUARDR(worldSize.x > 0 && worldSize.y > 0 && worldSize.z > 0, {})

    float const halfX = worldSize.x / 2.0f;
    float const halfY = worldSize.y / 2.0f;
    float const halfZ = worldSize.z / 2.0f;

    // Flattened cross: 4 cells wide, 3 cells tall
    float const uStep = 1.0f / 4.0f;
    float const vStep = 1.0f / 3.0f;

    VectorList<Vector3> vertices;
    VectorList<Vector2> uvs;
    VectorList<uint32_t> triangles;

    vertices.reserve(24);
    uvs.reserve(24);
    triangles.reserve(36);

    auto addFace = [&](Vector3 tl, Vector3 tr, Vector3 bl, Vector3 br, float uMin, float uMax,
                       float vMin, float vMax) {
        uint32_t base = (uint32_t)vertices.size();

        vertices.push_back(tl);
        vertices.push_back(tr);
        vertices.push_back(bl);
        vertices.push_back(br);

        uvs.push_back({ uMin, vMax });
        uvs.push_back({ uMax, vMax });
        uvs.push_back({ uMin, vMin });
        uvs.push_back({ uMax, vMin });

        triangles.push_back(base + 0);
        triangles.push_back(base + 1);
        triangles.push_back(base + 2);
        triangles.push_back(base + 1);
        triangles.push_back(base + 3);
        triangles.push_back(base + 2);
    };

    // +Z face (front): UV cell col 1, middle row
    addFace(
        { -halfX, +halfY, +halfZ }, { +halfX, +halfY, +halfZ }, { -halfX, -halfY, +halfZ },
        { +halfX, -halfY, +halfZ }, 1 * uStep, 2 * uStep, 1 * vStep, 2 * vStep
    );

    // +X face (right): UV cell col 2, middle row
    addFace(
        { +halfX, +halfY, +halfZ }, { +halfX, +halfY, -halfZ }, { +halfX, -halfY, +halfZ },
        { +halfX, -halfY, -halfZ }, 2 * uStep, 3 * uStep, 1 * vStep, 2 * vStep
    );

    // -Z face (back): UV cell col 3, middle row
    addFace(
        { +halfX, +halfY, -halfZ }, { -halfX, +halfY, -halfZ }, { +halfX, -halfY, -halfZ },
        { -halfX, -halfY, -halfZ }, 3 * uStep, 4 * uStep, 1 * vStep, 2 * vStep
    );

    // -X face (left): UV cell col 0, middle row
    addFace(
        { -halfX, +halfY, -halfZ }, { -halfX, +halfY, +halfZ }, { -halfX, -halfY, -halfZ },
        { -halfX, -halfY, +halfZ }, 0 * uStep, 1 * uStep, 1 * vStep, 2 * vStep
    );

    // +Y face (top): UV cell col 1, top row
    addFace(
        { -halfX, +halfY, -halfZ }, { +halfX, +halfY, -halfZ }, { -halfX, +halfY, +halfZ },
        { +halfX, +halfY, +halfZ }, 1 * uStep, 2 * uStep, 2 * vStep, 3 * vStep
    );

    // -Y face (bottom): UV cell col 1, bottom row
    addFace(
        { -halfX, -halfY, +halfZ }, { +halfX, -halfY, +halfZ }, { -halfX, -halfY, -halfZ },
        { +halfX, -halfY, -halfZ }, 1 * uStep, 2 * uStep, 0 * vStep, 1 * vStep
    );

    Mesh result;
    result.Update(vertices, triangles, uvs);

    return result;
}
