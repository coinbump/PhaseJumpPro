#pragma once

#include "SomeMeshBuilder.h"
#include "Vector2.h"
#include "Vector3.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/12/24
 */
namespace PJ {
    /// Abstract interface to an object that builds a poly frame mesh
    class SomePolyFrameMeshBuilder : public SomeMeshBuilder {
    public:
        virtual size_t PolyVertexCount() const = 0;

        /// Mesh requires both inner-frame and outer-frame vertices
        size_t MeshVertexCount() const {
            return PolyVertexCount() * 2;
        }
    };

    /// Builds a quad frame mesh
    class QuadFrameMeshBuilder : SomePolyFrameMeshBuilder {
    public:
        /// Mesh size
        Vector2 worldSize;

        /// Wuad frame border size
        Vector2 strokeSize;

        QuadFrameMeshBuilder(Vector2 worldSize, Vector2 strokeSize);

        // MARK: SomeMeshBuilder

        Mesh BuildMesh() override;

        // MARK: SomePolyFrameMeshBuilder

        size_t PolyVertexCount() const override {
            return 4;
        }
    };
} // namespace PJ
