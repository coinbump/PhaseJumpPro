#ifndef PJMESH_H
#define PJMESH_H

#include "Bounds.h"
#include "Vector2.h"
#include "Vector3.h"
#include "VectorList.h"
#include "Color.h"
#include "Rect.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/16/23
 */
namespace PJ {
    /// Stores properties for rendering a mesh
    struct Mesh
    {
        VectorList<Vector3> vertices;

        /// Used by index buffer (IBO) to keep vertices list smaller and improve performance
        VectorList<uint32_t> triangles;

        VectorList<RGBAColor> colors;
        VectorList<Vector2> uvs;

        Bounds bounds;

        void CalculateBounds() {
            bool hasFirst = false;
            Vector3 minValue;
            Vector3 maxValue;

            for (auto v : vertices) {
                if (!hasFirst) {
                    hasFirst = true;
                    minValue = v;
                    maxValue = v;
                    continue;
                }

                minValue = Vector3(std::min(minValue.x, v.x), std::min(minValue.y, v.y), std::min(minValue.z, v.z));
                maxValue = Vector3(std::max(maxValue.x, v.x), std::max(maxValue.y, v.y), std::max(maxValue.z, v.z));
            }

            Vector3 extents = (maxValue - minValue) / 2.0f;
            Vector3 center = minValue + extents;
            bounds = Bounds(center, extents);
        }

        void CalculateNormals() {
            // FUTURE: add normals support if needed
        }

        void OffsetBy(Vector2 offset) {
            for (auto & v : vertices) {
                v.x += offset.x;
                v.y += offset.y;
            }
        }

        void OffsetBy(Vector3 offset) {
            for (auto & v : vertices) {
                v.x += offset.x;
                v.y += offset.y;
                v.z += offset.z;
            }
        }

        /// Fit the uvs to the position and scale of the rect
        void FitUVsTo(Rect r) {
            // TODO:
        }

        void Update(std::optional<VectorList<Vector3>> vertices,
                    std::optional<VectorList<uint32_t>> triangles,
                    std::optional<VectorList<Vector2>> uvs)
        {
            if (vertices) {
                this->vertices = vertices.value();
            }

            if (triangles) {
                this->triangles = triangles.value();
            }

            if (uvs) {
                this->uvs = uvs.value();
            }

            CalculateBounds();
            CalculateNormals();
        }

        Mesh operator+(Mesh const& rhs) const
        {
            Mesh result = *this;
            result += rhs;
            return result;
        }

        Mesh& operator+=(Mesh const& rhs) {
            auto & result = *this;
            uint32_t triangleOffset = (uint32_t)result.vertices.size();

            result.vertices.AddRange(rhs.vertices);

            auto newTriangles = rhs.triangles;
            for (auto & triangle : newTriangles) {
                triangle += triangleOffset;
            }

            result.triangles.AddRange(newTriangles);

            result.colors.AddRange(rhs.colors);
            result.uvs.AddRange(rhs.uvs);

            result.CalculateBounds();
            result.CalculateNormals();

            return result;
        }
    };
}

#endif
