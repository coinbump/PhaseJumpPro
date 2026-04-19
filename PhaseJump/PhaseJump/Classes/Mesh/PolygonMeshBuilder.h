#pragma once

#include "Polygon.h"
#include "SomeMeshBuilder.h"

/*
 RATING: 5+ stars
 Tested and works
 CODE REVIEW: 4/18/26
 */
namespace PJ {
    /// Builds a filled mesh from a Polygon. UVs are normalized to the
    /// polygon's own bounds: the vertex at `polygon.Min()` maps to UV (0, 0)
    /// and the vertex at `polygon.Max()` maps to UV (1, 1).
    class PolygonMeshBuilder final : public SomeMeshBuilder {
    public:
        Polygon polygon;

        PolygonMeshBuilder() = default;

        PolygonMeshBuilder(Polygon polygon) :
            polygon(std::move(polygon)) {}

        // MARK: SomeMeshBuilder

        Mesh BuildMesh() override;
    };
} // namespace PJ
