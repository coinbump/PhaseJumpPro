#pragma once

#include "Angle.h"
#include "CenterPolyMeshBuilder.h"
#include "SomeMeshBuilder.h"

/*
 RATING: 5 stars
 Has unit tests (from CenterPolyMeshBuilder)
 CODE REVIEW: 12/13/24
 */
namespace PJ {
    /// Renders an arc
    class ArcMeshBuilder : public SomeMeshBuilder {
    public:
        Angle startAngle;
        Angle angleDelta{ { .degrees = 360 } };
        Angle angleStep{ { .degrees = 10 } };
        Vector2 worldSize{ 10, 10 };

        ArcMeshBuilder(Vector2 worldSize, Angle startAngle, Angle angleDelta, Angle angleStep) :
            startAngle(startAngle),
            angleDelta(angleDelta),
            angleStep(angleStep),
            worldSize(worldSize) {}

        // MARK: SomeMeshBuilder

        Mesh BuildMesh() override;
    };
} // namespace PJ
