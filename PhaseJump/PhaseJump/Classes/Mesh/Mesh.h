#pragma once

#include "Bounds.h"
#include "Color.h"
#include "Macros.h"
#include "Matrix4x4.h"
#include "Rect.h"
#include "SharedVector.h"
#include "Vector2.h"
#include "Vector3.h"
#include "VectorList.h"
#include <algorithm>
#include <span>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/16/23
 */
namespace PJ {
    /// Stores properties for rendering a mesh
    class Mesh {
    public:
        using This = Mesh;
        using TrianglesSpan = std::span<uint32_t const>;
        using CalculateBoundsFunc = std::function<Bounds(This&)>;

    protected:
        bool needsCalculate = true;

        /// Texture coordinate for each vertex
        VectorList<Vector2> uvs;

        /// Used by index buffer (IBO) to keep vertices list smaller and improve
        /// performance
        SharedVector<uint32_t> triangles;

        Bounds bounds;

        void CalculateBounds() {
            GUARD(calculateBoundsFunc)
            bounds = calculateBoundsFunc(*this);
        }

        void CalculateNormals() {
            // FUTURE: add normals support if needed
        }

        VectorList<Vector3> vertices;

    public:
        CalculateBoundsFunc calculateBoundsFunc;

        Mesh();

        /// Using initializer list is faster than copying in the vertices
        Mesh(std::initializer_list<Vector3> const& vertices) :
            vertices(vertices) {}

        void SetNeedsCalculate() {
            needsCalculate = true;
        }

        Bounds GetBounds() const {
            // This is a cheat, but otherwise we spread non-const everywhere
            const_cast<Mesh*>(this)->CalculateIfNeeded();
            return bounds;
        }

        constexpr VectorList<Vector3> const& Vertices() const {
            return vertices;
        }

        /// When used, you take responsibility for recaculating the bounds and normals later
        VectorList<Vector3>& ModifiableVertices() {
            return vertices;
        }

        void SetVertices(VectorList<Vector3> const& value) {
            vertices = value;
            needsCalculate = true;
        }

        constexpr VectorList<Vector2> const& UVs() const {
            return uvs;
        }

        constexpr VectorList<Vector2>& UVs() {
            return uvs;
        }

        void SetUVs(VectorList<Vector2> const& value) {
            uvs = value;
        }

        constexpr std::span<uint32_t const> Triangles() const {
            // return sharedTriangles ? *sharedTriangles : triangles;
            return triangles.Value();
        }

        void SetTriangles(std::span<uint32_t const> value) {
            triangles = value;
        }

        void SetModifiableTriangles(VectorList<uint32_t> const& value) {
            triangles = value;
        }

        VectorList<uint32_t>& ModifiableTriangles() {
            return triangles.Modifiable();
        }

        This& OffsetBy(Vector2 offset) {
            for (auto& v : vertices) {
                v.x += offset.x;
                v.y += offset.y;
            }

            return *this;
        }

        This& OffsetBy(Vector3 offset) {
            for (auto& v : vertices) {
                v.x += offset.x;
                v.y += offset.y;
                v.z += offset.z;
            }

            return *this;
        }

        /// Fit the uvs to the position and scale of the rect
        // FUTURE: void FitUVsTo(Rect r)

        void Update(
            VectorList<Vector3> const& vertices, VectorList<uint32_t> const& triangles,
            VectorList<Vector2> const& uvs
        );

        void CalculateIfNeeded();

        Mesh operator+(Mesh const& rhs) const;
        Mesh& operator+=(Mesh const& rhs);
        Mesh& operator*=(Matrix4x4 const& rhs);
        Mesh operator*(Matrix4x4 const& rhs) const;
    };
} // namespace PJ
