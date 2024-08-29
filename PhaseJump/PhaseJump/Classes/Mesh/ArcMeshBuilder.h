#pragma once

#include "Angle.h"
#include "CenterPolyMeshBuilder.h"
#include "SomeMeshBuilder.h"

/*
 RATING: 4 stars
 Tested and works (8/25/24). Needs unit tests
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    /// Renders an arc
    class ArcMeshBuilder : public SomeMeshBuilder {
    public:
        Angle startAngle = Angle::DegreesAngle(0);
        Angle endAngle = Angle::DegreesAngle(360);
        Angle angleStep = Angle::DegreesAngle(10);
        Vector2 worldSize = Vector2(10, 10);

        ArcMeshBuilder(Vector2 worldSize, Angle startAngle, Angle endAngle, Angle angleStep) :
            startAngle(startAngle),
            endAngle(endAngle),
            angleStep(angleStep),
            worldSize(worldSize) {}

        size_t SliceCount() const;

        Mesh BuildMesh() override;
    };
} // namespace PJ
