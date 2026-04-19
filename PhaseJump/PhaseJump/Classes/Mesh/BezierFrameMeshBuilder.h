#pragma once

#include "BezierPath.h"
#include "SomeMeshBuilder.h"

// TODO: code review
namespace PJ {
    /// Builds a frame (stroke) mesh for a Bezier path by sampling the curve
    /// into a polyline and delegating to `PolyFrameMeshBuilder`.
    class BezierFrameMeshBuilder final : public SomeMeshBuilder {
    public:
        struct Config {
            BezierPath bezierPath;

            /// Target world-space distance between sampled points along the curve.
            /// Smaller values produce a smoother frame at higher mesh cost.
            float segmentDistance = 10;

            float strokeWidth = 1;

            PathCap startCap = PathCap::Flat;
            PathCap endCap = PathCap::Flat;
        };

        BezierPath bezierPath;
        float segmentDistance = 10;
        float strokeWidth = 1;
        PathCap startCap = PathCap::Flat;
        PathCap endCap = PathCap::Flat;

        BezierFrameMeshBuilder(Config const& config);

        // MARK: SomeMeshBuilder

        Mesh BuildMesh() override;
    };
} // namespace PJ
