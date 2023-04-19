#ifndef PJRENDERMESH_H
#define PJRENDERMESH_H

#include "Bounds.h"
#include "Vector2.h"
#include "Vector3.h"
#include "VectorList.h"
#include "Color.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/16/23
 */
namespace PJ {
    /// Stores properties for rendering a mesh
    /// Unity Equivalent: Mesh
    struct RenderMesh
    {
        VectorList<Vector3> vertices;

        /// Used by index buffer (IBO) to keep vertices list smaller and improve performance
        VectorList<uint32_t> triangles;

        VectorList<Color32> colors;
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
    };
}

#endif
