#pragma once

#include "Axis.h"
#include "SomeMeshBuilder.h"
#include "Vector2.h"
#include "Vector3.h"

/*
 RATING: 4 stars
 Tested and works
 CODE REVIEW: 12/22/24
 */
namespace PJ {
    /// A plane mesh of NxN cells built across the x,y, or z axis
    class PlaneMeshBuilder : public SomeMeshBuilder {
    public:
        Vector2Int meshSize;
        Vector2 worldSize;
        Axis faceAxis = Axis::Z;

        struct Config {
            Vector2Int meshSize{ 3, 3 };
            Vector2 worldSize{ 10, 10 };
            Axis faceAxis = Axis::Z;
        };

        PlaneMeshBuilder(Config config);

        // MARK: SomeMeshBuilder

        Mesh BuildMesh() override;
    };
} // namespace PJ
