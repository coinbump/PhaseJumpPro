#pragma once

#include "SomeMeshBuilder.h"
#include "Vector3.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/19/26
 */
namespace PJ {
    /// Box mesh with 24 vertices (4 per face, 1 quad per face).
    /// UVs use the flattened-cross net layout (4 cells wide, 3 cells tall)
    class BoxMeshBuilder final : public SomeMeshBuilder {
    public:
        Vector3 worldSize{};

        BoxMeshBuilder(Vector3 worldSize = Vector3::Uniform(1));

        // MARK: SomeMeshBuilder

        Mesh BuildMesh() override;
    };
} // namespace PJ
