#pragma once

#include "SomeMeshBuilder.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/12/24
 */
namespace PJ {
    namespace QuadMesh {
        extern VectorList<uint32_t> const triangles;
        extern VectorList<Vector2> const uvs;
    } // namespace QuadMesh

    /// 1x1 quad mesh, oriented to x-y plane
    class QuadMeshBuilder : public SomeMeshBuilder {
    public:
        Vector2 worldSize{};

        QuadMeshBuilder(Vector2 worldSize = Vector2::one);

        // MARK: SomeMeshBuilder

        Mesh BuildMesh() override;
    };
} // namespace PJ
