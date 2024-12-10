//
//  QuadMeshBuilder.cpp
//  PhaseJump
//
//  Created by Jeremy Vineyard on 9/3/24.
//

#include "QuadMeshBuilder.h"
#include "VectorList.h"

using namespace std;
using namespace PJ;

VectorList<uint32_t> const QuadMesh::triangles{ 0, 1, 2, 1, 3, 2 };

// It only makes sense to share these if all textures are the same
VectorList<Vector2> const QuadMesh::uvs{ Vector2(0, 0), Vector2(1, 0), Vector2(0, 1),
                                         Vector2(1, 1) };

QuadMeshBuilder::QuadMeshBuilder(Vector2 worldSize) :
    worldSize(worldSize) {}

Mesh QuadMeshBuilder::BuildMesh() {
    float halfX = worldSize.x / 2.0f;
    float halfY = worldSize.y / 2.0f;

    Mesh result({ { halfX * vecLeft, halfY * vecUp, 0 },
                  { halfX * vecRight, halfY * vecUp, 0 },
                  { halfX * vecLeft, halfY * vecDown, 0 },
                  { halfX * vecRight, halfY * vecDown, 0 } });

    // FUTURE: this is an interesting idea, because copying a pointer is faster than a
    // vector copy But it adds more complexity and edge cases elsewhere. Re-evaluate later
    // result.SetSharedTriangles(&QuadMesh::triangles);

    result.SetTriangles(Mesh::TrianglesSpan(QuadMesh::triangles));
    result.SetUVs(QuadMesh::uvs);

    return result;
}
