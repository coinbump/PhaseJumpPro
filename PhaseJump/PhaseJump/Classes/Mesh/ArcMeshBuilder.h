#pragma once

#include "Angle.h"
#include "CenterPolyMeshBuilder.h"
#include "SomeMeshBuilder.h"

/*
 RATING: 5+ stars
 Has unit tests
 CODE REVIEW: 4/18/26
 */
namespace PJ {
    /// Renders an arc
    class ArcMeshBuilder final : public SomeMeshBuilder {
    public:
        Angle startAngle;
        Angle angleDelta;
        Angle angleStep;
        Vector2 worldSize;

        struct Config {
            Vector2 worldSize{ 10, 10 };
            Angle startAngle;
            Angle angleDelta = Angle::WithDegrees(360);
            Angle angleStep = Angle::WithDegrees(10);
        };

        ArcMeshBuilder(Config const& config);

        // MARK: SomeMeshBuilder

        Mesh BuildMesh() override;
    };
} // namespace PJ
