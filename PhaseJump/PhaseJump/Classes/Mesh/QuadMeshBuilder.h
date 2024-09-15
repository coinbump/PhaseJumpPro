#pragma once

#include "PlaneMeshBuilder.h"
#include "SomeMeshBuilder.h"

/*
 RATING: 5 stars
 Tested and works (8/25/24)
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    namespace QuadMesh {
        extern VectorList<uint32_t> const triangles;
        extern VectorList<Vector2> const uvs;
    } // namespace QuadMesh

    /// 1x1 quad mesh, oriented to x-y plane
    class QuadMeshBuilder : public SomeMeshBuilder {
    public:
        Vector2 worldSize{ 10.0f, 10.0f };

        QuadMeshBuilder(Vector2 worldSize = vec2One) :
            worldSize(worldSize) {}

        Mesh BuildMesh() override {
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
    };
} // namespace PJ
