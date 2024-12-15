#pragma once

#include "Angle.h"
#include "Axis.h"
#include "SomeMeshBuilder.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/14/24
 */
namespace PJ {
    /// Defines size of round corners
    struct RoundCorners {
        Vector2 topLeft{ 20, 20 };
        Vector2 topRight{ 20, 20 };
        Vector2 bottomLeft{ 20, 20 };
        Vector2 bottomRight{ 20, 20 };

        static RoundCorners WithRadius(float radius) {
            return { .topLeft = { radius, radius },
                     .topRight = { radius, radius },
                     .bottomLeft = { radius, radius },
                     .bottomRight = { radius, radius } };
        }
    };

    /// Builds a horizontal capsule mesh (capsules are rarely vertical)
    class RoundCornersMeshBuilder : public SomeMeshBuilder {
    public:
        Vector2 worldSize = { 10, 10 };
        Angle angleStep = Angle::WithDegrees(10);
        RoundCorners corners;

        struct Config {
            RoundCorners corners;
            Vector2 worldSize{ 10, 10 };
            Angle angleStep = Angle::WithDegrees(10);
        };

        RoundCornersMeshBuilder(Config config);

        // MARK: SomeMeshBuilder

        Mesh BuildMesh() override;
    };
} // namespace PJ
