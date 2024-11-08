#pragma once

#include "Angle.h"
#include "CenterPolyBuilder.h"
#include "Mesh.h"
#include "SomeMeshBuilder.h"

/*
 RATING: 4 stars
 Tested and works (8/25/24). Needs unit tests
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    /// A mesh that can be defined by a center point at (0, 0) and a surrounding
    /// path Example: capsule, rounded rectangle, arc, ellipse, circle
    class CenterPolyMeshBuilder : public SomeMeshBuilder {
    public:
        Vector2 worldSize;
        CenterPolyShape shape;

        CenterPolyMeshBuilder(Vector2 worldSize, CenterPolyShape shape) :
            worldSize(worldSize),
            shape(shape) {}

        size_t MeshVertexCount(Polygon const& poly) const {
            return poly.Count() + 1;
        }

        size_t SliceCount(Polygon const& poly) const {
            GUARDR(!IsEmpty(poly.value), 0)
            return poly.Count() - 1;
        }

        Mesh BuildMesh() override {
            Mesh result;

            auto poly = CenterPolyBuilder().Build(vec2Zero, worldSize, shape);

            if (poly.Count() < 2) {
                return result;
            }

            auto vertexCount = MeshVertexCount(poly);

            VectorList<Vector3> vertices(vertexCount, Vector3::zero);
            VectorList<Vector2> uvs(vertexCount, vec2Zero);

            auto sliceCount = SliceCount(poly);
            auto trianglesCount = sliceCount * 3;

            VectorList<uint32_t> triangles(trianglesCount, 0);

            // Center vertex
            vertices[0] = Vector3::zero;
            uvs[0] = Vector2(0.5f, 0.5f);

            Polygon polyWithCenter;
            Add(polyWithCenter.value, vertices[0]);
            AddRange(polyWithCenter.value, poly.value);
            auto polygonMin = polyWithCenter.Min();
            auto polygonSize = polyWithCenter.Size();

            // Edge vertices
            int vi = 1;
            for (auto& vertex : poly.value) {
                vertices[vi] = vertex;
                uvs[vi] = Vector2(
                    (vertex.x - polygonMin.x) / polygonSize.x,
                    (vertex.y - polygonMin.y) / polygonSize.y
                );
                vi++;
            }

            auto offset = 0;
            for (int i = 0; i < sliceCount; i++) {
                triangles[offset] = 0;
                triangles[offset + 1] = i + 1;
                triangles[offset + 2] = i + 2;

                offset += 3;
            }

            result.Update(vertices, triangles, uvs);

            return result;
        }
    };
} // namespace PJ
