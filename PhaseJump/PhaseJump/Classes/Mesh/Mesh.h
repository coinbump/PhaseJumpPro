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

    protected:
        bool needsCalculate = true;

        /// Texture coordinate for each vertex
        VectorList<Vector2> uvs;

        /// Used by index buffer (IBO) to keep vertices list smaller and improve
        /// performance
        SharedVector<uint32_t> triangles;

        Bounds bounds;

        void CalculateBounds() {
            bool hasFirst = false;
            Vector3 minValue;
            Vector3 maxValue;

            for (auto& v : vertices) {
                if (!hasFirst) {
                    hasFirst = true;
                    minValue = v;
                    maxValue = v;
                    continue;
                }

                minValue = Vector3(
                    std::min(minValue.x, v.x), std::min(minValue.y, v.y), std::min(minValue.z, v.z)
                );
                maxValue = Vector3(
                    std::max(maxValue.x, v.x), std::max(maxValue.y, v.y), std::max(maxValue.z, v.z)
                );
            }

            Vector3 extents = (maxValue - minValue) / 2.0f;
            Vector3 center = minValue + extents;
            bounds = Bounds(center, extents);
        }

        void CalculateNormals() {
            // FUTURE: add normals support if needed
        }

        VectorList<Vector3> vertices;

    public:
        Mesh() {}

        /// Using initializer list is faster than copying in the vertices
        Mesh(std::initializer_list<Vector3> const& vertices) :
            vertices(vertices) {}

        void SetNeedsCalculate() {
            needsCalculate = true;
        }

        constexpr Bounds GetBounds() const {
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

        void OffsetBy(Vector2 offset) {
            for (auto& v : vertices) {
                v.x += offset.x;
                v.y += offset.y;
            }
        }

        void OffsetBy(Vector3 offset) {
            for (auto& v : vertices) {
                v.x += offset.x;
                v.y += offset.y;
                v.z += offset.z;
            }
        }

        /// Fit the uvs to the position and scale of the rect
        // FUTURE: void FitUVsTo(Rect r)

        void Update(
            VectorList<Vector3> const& vertices, VectorList<uint32_t> const& triangles,
            VectorList<Vector2> const& uvs
        ) {
            this->vertices = vertices;
            this->triangles = triangles;
            this->uvs = uvs;

            needsCalculate = true;
        }

        void CalculateIfNeeded() {
            GUARD(needsCalculate)
            needsCalculate = false;

            CalculateBounds();
            CalculateNormals();
        }

        Mesh operator+(Mesh const& rhs) const {
            Mesh result = *this;
            result += rhs;
            return result;
        }

        Mesh& operator+=(Mesh const& rhs) {
            auto& result = *this;
            uint32_t triangleOffset = (uint32_t)result.vertices.size();

            AddRange(result.vertices, rhs.vertices);

            VectorList<uint32_t> newTriangles;
            newTriangles.assign(rhs.Triangles().begin(), rhs.Triangles().end());
            for (auto& triangle : newTriangles) {
                triangle += triangleOffset;
            }

            AddRange(result.triangles.Modifiable(), newTriangles);
            AddRange(result.uvs, rhs.uvs);

            result.needsCalculate = true;

            return result;
        }

        Mesh& operator*=(Matrix4x4 const& rhs) {
            auto& result = *this;
            std::transform(
                result.vertices.begin(), result.vertices.end(), result.vertices.begin(),
                [&](Vector3 v) { return rhs.MultiplyPoint(v); }
            );
            return result;
        }

        Mesh operator*(Matrix4x4 const& rhs) const {
            Mesh result = *this;
            result *= rhs;
            return result;
        }
    };
} // namespace PJ
