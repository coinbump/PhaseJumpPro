#ifndef PJQUADMESHBUILDER_H
#define PJQUADMESHBUILDER_H

#include "PlaneMeshBuilder.h"
#include "SomeMeshBuilder.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/16/23
 */
namespace PJ {
    /// A plane mesh that is oriented to xy space, and defaults to 1x1 size
    class QuadMeshBuilder : public SomeMeshBuilder {
    public:
        Vector2Int meshSize{ 1, 1 };
        Vector2 worldSize{ 1.0f, 1.0f };

        QuadMeshBuilder(Vector2 worldSize = Vector2::one, Vector2Int meshSize = Vector2Int::one) :
            worldSize(worldSize),
            meshSize(meshSize) {}

        Mesh BuildMesh() override {
            return PlaneMeshBuilder(worldSize, meshSize, Axis::Y).BuildMesh();
        }
    };
} // namespace PJ

#endif
