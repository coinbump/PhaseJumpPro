#ifndef PJMESH_H
#define PJMESH_H

#include "Bounds.h"
#include "Color.h"
#include "Macros.h"
#include "Matrix4x4.h"
#include "Rect.h"
#include "Vector2.h"
#include "Vector3.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/16/23
 */
namespace PJ {
    /// Stores properties for rendering a mesh
    struct Mesh {
    public:
        using This = Mesh;

    protected:
        bool needsCalculate = true;

        /// Texture coordinate for each vertex
        VectorList<Vector2> uvs;

        /// Used by index buffer (IBO) to keep vertices list smaller and improve
        /// performance
        VectorList<uint32_t> triangles;

        // FUTURE: this is an interesting idea, because copying a pointer is faster than a vector
        // copy But it adds more complexity and edge cases elsewhere. Re-evaluate later
        /// Copying vectors is expensive for large lists. Use to share indices between meshes
        // VectorList<uint32_t> const* sharedTriangles = nullptr;

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

        constexpr VectorList<uint32_t> const& Triangles() const {
            // return sharedTriangles ? *sharedTriangles : triangles;
            return triangles;
        }

        void SetTriangles(VectorList<uint32_t> const& value) {
            triangles = value;
        }

        // TODO: re-evaluate the idea of shared triangles (can we abstract this with CRTP? What
        // about add operation and batch?)
        VectorList<uint32_t>& BaseTriangles() {
            return triangles;
        }

        // FUTURE: this is an interesting idea, because copying a pointer is faster than a vector
        // copy But it adds more complexity and edge cases elsewhere. Re-evaluate later
        //        This& SetSharedTriangles(VectorList<uint32_t> const* value) {
        //            sharedTriangles = value;
        //            return *this;
        //        }

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

            auto newTriangles = rhs.Triangles();
            for (auto& triangle : newTriangles) {
                triangle += triangleOffset;
            }

            AddRange(result.triangles, newTriangles);
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

#endif
