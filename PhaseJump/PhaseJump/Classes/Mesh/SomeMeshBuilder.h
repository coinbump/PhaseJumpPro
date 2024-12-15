#pragma once

#include "Mesh.h"
#include "VertexList.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 12/13/24
 */
namespace PJ {
    // FUTURE: support Square line cap
    /// Defines how line start/end caps are generated
    enum class PathCap { Flat, Round };

    /// Defines how path corners are generated
    enum class PathCorner { None, Bevel, Round, Miter };

    /// Builds a render mesh with vertices
    class SomeMeshBuilder {
    public:
        virtual ~SomeMeshBuilder() {}

        virtual Mesh BuildMesh() = 0;
    };
} // namespace PJ
