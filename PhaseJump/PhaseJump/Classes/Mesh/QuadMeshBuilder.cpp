#include "QuadMeshBuilder.h"
#include "VectorList.h"

using namespace std;
using namespace PJ;

VectorList<uint32_t> const QuadMesh::triangles{ 0, 1, 2, 1, 3, 2 };

// It only makes sense to share these if all textures are the same
VectorList<Vector2> const QuadMesh::uvs{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } };

QuadMeshBuilder::QuadMeshBuilder(Vector2 worldSize) :
    worldSize(worldSize) {}

Mesh QuadMeshBuilder::BuildMesh() {
    GUARDR(worldSize.x > 0 && worldSize.y > 0, {})

    float halfX = worldSize.x / 2.0f;
    float halfY = worldSize.y / 2.0f;

    Mesh result({ { halfX * vecLeft, halfY * vecUp, 0 },
                  { halfX * vecRight, halfY * vecUp, 0 },
                  { halfX * vecLeft, halfY * vecDown, 0 },
                  { halfX * vecRight, halfY * vecDown, 0 } });

    result.SetTriangles(QuadMesh::triangles);
    result.SetUVs(QuadMesh::uvs);

    return result;
}
