#pragma once

#include "Angle.h"
#include "ArcMeshBuilder.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    /// Renders an ellipse
    class EllipseMeshBuilder : public SomeMeshBuilder {
    public:
        Angle angleStep = Angle::DegreesAngle(10.0f);
        Vector2 worldSize = Vector2(1.0f, 1.0f);

        EllipseMeshBuilder(Vector2 worldSize, Angle angleStep = Angle::DegreesAngle(10)) :
            angleStep(angleStep),
            worldSize(worldSize) {}

        Mesh BuildMesh() override {
            return ArcMeshBuilder(
                       worldSize, Angle::DegreesAngle(0), Angle::DegreesAngle(360.0f), angleStep
            )
                .BuildMesh();
        }
    };
} // namespace PJ
