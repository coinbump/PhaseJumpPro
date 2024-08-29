#pragma once

#include "Mesh.h"
#include "VertexList.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 8/8/24
 */
namespace PJ {
    /// Builds a render mesh with vertices
    class SomeMeshBuilder {
    public:
        virtual ~SomeMeshBuilder() {}

        virtual Mesh BuildMesh() = 0;
    };
} // namespace PJ
