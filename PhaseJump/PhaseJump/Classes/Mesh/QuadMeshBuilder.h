#pragma once

#include "PlaneMeshBuilder.h"
#include "SomeMeshBuilder.h"

/*
 RATING: 5 stars
 Tested and works (8/25/24)
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    /// A plane mesh that is oriented to xy space, and defaults to 1x1 size
    class QuadMeshBuilder : public SomeMeshBuilder {
    public:
        Vector2Int meshSize{ 1, 1 };
        Vector2 worldSize{ 10.0f, 10.0f };

        QuadMeshBuilder(Vector2 worldSize = Vector2::one, Vector2Int meshSize = Vector2Int::one) :
            worldSize(worldSize),
            meshSize(meshSize) {}

        Mesh BuildMesh() override {
            return PlaneMeshBuilder(worldSize, meshSize, Axis::Y).BuildMesh();
        }
    };
} // namespace PJ
