#pragma once

#include "CenterPolyBuilder.h"
#include "Macros.h"
#include "SomeMeshBuilder.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/14/24
 */
namespace PJ {
    /// Builds a frame mesh for a uniform poly that extends from the center
    class CenterPolyFrameMeshBuilder : public SomeMeshBuilder {
    public:
        struct Config {
            Vector2 worldSize;
            float strokeWidth = 1;
            CenterPolyModel model;

            PathCap startCap = PathCap::Flat;
            PathCap endCap = PathCap::Flat;
        };

        Vector2 worldSize;
        float strokeWidth = 1;
        CenterPolyModel model;

        PathCap startCap = PathCap::Flat;
        PathCap endCap = PathCap::Flat;

        CenterPolyFrameMeshBuilder(Config config);

        // MARK: SomeMeshBuilder

        Mesh BuildMesh() override;
    };
} // namespace PJ
