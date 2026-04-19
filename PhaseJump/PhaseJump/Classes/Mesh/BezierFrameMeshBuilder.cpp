#include "BezierFrameMeshBuilder.h"
#include "PolyFrameMeshBuilder.h"
#include "Polygon.h"
#include <algorithm>
#include <cmath>

using namespace PJ;

namespace {
    /// Sample count used to estimate the curve's total length before choosing how
    /// many segments to emit. Higher = more accurate length estimate.
    constexpr int lengthSampleCount = 64;
} // namespace

BezierFrameMeshBuilder::BezierFrameMeshBuilder(Config const& config) :
    bezierPath(config.bezierPath),
    segmentDistance(config.segmentDistance),
    strokeWidth(config.strokeWidth),
    startCap(config.startCap),
    endCap(config.endCap) {}

Mesh BezierFrameMeshBuilder::BuildMesh() {
    GUARDR(segmentDistance > 0, {})
    GUARDR(bezierPath.controlPoints.size() >= 3, {})

    // Estimate total arc length by densely sampling at uniform t.
    float totalLength = 0;
    Vector3 prevPoint = bezierPath.PositionAt(0);
    for (int i = 1; i <= lengthSampleCount; i++) {
        float t = (float)i / (float)lengthSampleCount;
        Vector3 point = bezierPath.PositionAt(t);
        totalLength += (point - prevPoint).Magnitude();
        prevPoint = point;
    }

    GUARDR(totalLength > 0, {})

    // Choose the smallest segment count whose per-segment length <= segmentDistance.
    int segmentCount = std::max(1, (int)std::ceil(totalLength / segmentDistance));

    // Build the polyline (segmentCount + 1 vertices) in 2D — the frame lives in the XY plane.
    Polygon poly;
    for (int i = 0; i <= segmentCount; i++) {
        float t = (float)i / (float)segmentCount;
        Vector3 point = bezierPath.PositionAt(t);
        poly.Add(Vector2(point.x, point.y));
    }

    PolyFrameMeshBuilder polyBuilder({ .poly = poly,
                                       .strokeWidth = strokeWidth,
                                       .polyClose = PolyClose::Open,
                                       .startCap = startCap,
                                       .endCap = endCap });

    return polyBuilder.BuildMesh();
}
