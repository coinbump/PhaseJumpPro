#pragma once

#include "Angle.h"
#include "ArcMeshBuilder.h"

/*
 RATING: 5 stars
 Has unit tests (from CenterPolyMeshBuilder)
 CODE REVIEW: 12/13/24
 */
namespace PJ {
    /// Renders an ellipse
    class EllipseMeshBuilder : public SomeMeshBuilder {
    public:
        Angle angleStep{ { .degrees = 10 } };
        Vector2 worldSize{ 1, 1 };

        EllipseMeshBuilder(Vector2 worldSize, Angle angleStep = Angle::WithDegrees(10)) :
            angleStep(angleStep),
            worldSize(worldSize) {}

        // MARK: SomeMeshBuilder

        Mesh BuildMesh() override {
            return ArcMeshBuilder(
                       worldSize, Angle::WithDegrees(0), Angle::WithDegrees(360.0f), angleStep
            )
                .BuildMesh();
        }
    };
} // namespace PJ
