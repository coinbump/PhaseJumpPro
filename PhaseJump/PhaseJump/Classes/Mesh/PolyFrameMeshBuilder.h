#pragma once

#include "Polygon.h"
#include "SomeMeshBuilder.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/14/24
 */
namespace PJ {
    /**
     Builds a mesh that shows the frame of a polygon

     FUTURE: support miter, smooth corners, end caps, etc.
     */
    class PolyFrameMeshBuilder : SomeMeshBuilder {
    public:
        struct Config {
            Polygon poly;
            float strokeWidth = 1;
            PolyClose polyClose = PolyClose::Closed;
            PathCap startCap = PathCap::Flat;
            PathCap endCap = PathCap::Flat;
            PathCorner corner = PathCorner::None;
        };

        float strokeWidth = 1;

        /// Determines how to render the final segment (from final vertex to last vertex)
        PolyClose polyClose = PolyClose::Closed;

        Polygon poly;

        PathCap startCap = PathCap::Flat;
        PathCap endCap = PathCap::Flat;

        // FUTURE: WORK IN PROGRESS (unfinished)
        PathCorner corner = PathCorner::None;

        PolyFrameMeshBuilder(Config config);

        // MARK: SomeMeshBuilder

        Mesh BuildMesh() override;
    };
} // namespace PJ
