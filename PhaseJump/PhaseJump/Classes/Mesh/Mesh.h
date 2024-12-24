#pragma once

#include "Bounds.h"
#include "Color.h"
#include "Macros.h"
#include "Matrix4x4.h"
#include "Rect.h"
#include "SharedVectorList.h"
#include "Vector2.h"
#include "Vector3.h"
#include "VectorList.h"
#include <algorithm>
#include <span>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/22/24
 */
namespace PJ {
    /// Stores properties for rendering a mesh
    class Mesh {
    public:
        using This = Mesh;
        using CalculateBoundsFunc = std::function<Bounds(This const&)>;

    protected:
        /// If true, associate properties for the mesh like bounds and normals need to be
        /// recalculated
        mutable bool needsCalculate = true;

        /// Mesh bounds, calculated on demand
        mutable Bounds bounds;

        // FUTURE: calculate UVs automatically based on vertex position and mesh bounds
        /// Texture coordinate for each vertex
        VectorList<Vector2> uvs;

        VectorList<uint32_t> triangles;

        void CalculateBounds() const {
            GUARD(calculateBoundsFunc)
            bounds = calculateBoundsFunc(*this);
        }

        void CalculateNormals() const {
            // FUTURE: add normals support if needed
        }

        VectorList<Vector3> vertices;

    public:
        CalculateBoundsFunc calculateBoundsFunc;

        Mesh();

        Mesh(std::initializer_list<Vector3> const& vertices) :
            vertices(vertices) {}

        /// Force associate properties (bounds, normals) to be recalculated the next time they are
        /// requested
        void SetNeedsCalculate() {
            needsCalculate = true;
        }

        Bounds GetBounds() const {
            CalculateIfNeeded();
            return bounds;
        }

        VectorList<Vector3> const& Vertices() const {
            return vertices;
        }

        /// After modifying vertices, call SetNeedsCalculate
        VectorList<Vector3>& Vertices() {
            return vertices;
        }

        /// Updates the vertices for this mesh
        void SetVertices(VectorList<Vector3> const& value) {
            vertices = value;
            needsCalculate = true;
        }

        VectorList<Vector2> const& UVs() const {
            return uvs;
        }

        VectorList<Vector2>& UVs() {
            return uvs;
        }

        void SetUVs(VectorList<Vector2> const& value) {
            uvs = value;
        }

        VectorList<uint32_t> const& Triangles() const {
            return triangles;
        }

        VectorList<uint32_t>& Triangles() {
            return triangles;
        }

        void SetTriangles(VectorList<uint32_t> const& value) {
            triangles = value;
        }

        /// Offsets mesh vertices
        This& Offset(Vector3 offset);

        /// Fit the uvs to the position and scale of the rect
        // FUTURE: void FitUVsTo(Rect r)

        void Update(
            VectorList<Vector3> const& vertices, VectorList<uint32_t> const& triangles,
            VectorList<Vector2> const& uvs
        );

        void CalculateIfNeeded() const;

        Mesh operator+(Mesh const& rhs) const;
        Mesh& operator+=(Mesh const& rhs);
        Mesh& operator*=(Matrix4x4 const& rhs);
        Mesh operator*(Matrix4x4 const& rhs) const;
    };
} // namespace PJ
