#pragma once

#include "CenterPolyBuilder.h"
#include "Macros.h"
#include "SomeMeshBuilder.h"

/*
 TODO: BUG: doesn't build frames correctly (try ::square, ::hex)
 */
namespace PJ {
    /// Builds a frame mesh for a uniform poly that extends from the center
    class CenterPolyFrameMeshBuilder : public SomeMeshBuilder {
    public:
        Vector2 worldSize;
        float strokeWidth = 0;
        CenterPolyShape shape;

        CenterPolyFrameMeshBuilder(Vector2 worldSize, float strokeWidth, CenterPolyShape shape) :
            worldSize(worldSize),
            strokeWidth(strokeWidth),
            shape(shape) {}

        size_t MeshVertexCount(Polygon const& poly) const {
            return (poly.size()) * 2;
        }

        size_t SliceCount(Polygon const& poly) const {
            return poly.size() - 1;
        }

        Mesh BuildMesh() override {
            Mesh result;

            auto poly = CenterPolyBuilder().Build(Vector2::zero, worldSize, shape);

            if (poly.size() < 2) {
                return result;
            }

            auto vertexCount = MeshVertexCount(poly);
            VectorList<Vector3> vertices(vertexCount, Vector3::zero);
            auto uvCount = vertexCount;
            VectorList<Vector2> uvs(uvCount, Vector2::zero);
            auto sliceCount = SliceCount(poly);
            auto trianglesCount = sliceCount * 6;
            VectorList<uint32_t> triangles(trianglesCount, 0);

            Polygon polyWithCenter;
            polyWithCenter.push_back(Vector3::zero);
            AddRange(polyWithCenter, poly);
            auto polygonMin = polyWithCenter.Min();
            auto polygonSize = polyWithCenter.Size();

            // Edge vertices
            int vi = 0;
            for (auto& vertex : poly) {
                vertices[vi] = vertex;
                uvs[vi] = Vector2(
                    (vertex.x - polygonMin.x) / polygonSize.x,
                    (vertex.y - polygonMin.y) / polygonSize.y
                );
                vi++;
            }

            auto firstVi = vi;
            Vector3 prevVertex = Vector3::zero;
            bool hasPrevVertex = false;
            size_t index = 0;

            // This has only been tested with closed shapes
            for (auto& vertex : poly) {
                auto thisVertex = vertex;

                Vector3 reference = vertex;
                //                if (poly.IsClosed()) {
                //                    reference = poly[poly.size() - 2];
                //                }
                if (hasPrevVertex) {
                    reference = prevVertex;
                }

                prevVertex = vertex;
                hasPrevVertex = true;

                auto lineVector = vertex - reference;
                Angle lineAngle(lineVector);

                auto nextLineVertex = poly[poly.size() - 1];
                if (index + 1 < poly.size()) {
                    nextLineVertex = poly[index + 1];
                }

                auto nextLineVector = nextLineVertex - vertex;
                Angle nextLineAngle(nextLineVector);

                auto innerOffset =
                    Angle::DegreesAngle(lineAngle.Degrees() + 90.0f).ToVector2(strokeWidth) / 2.0f +
                    Angle::DegreesAngle(nextLineAngle.Degrees() + 90.0f).ToVector2(strokeWidth) /
                        2.0f;
                thisVertex += Vector3(innerOffset.x, innerOffset.y, 0);

                vertices[vi] = thisVertex;
                uvs[vi] = Vector2(
                    (thisVertex.x - polygonMin.x) / polygonSize.x,
                    (thisVertex.y - polygonMin.y) / polygonSize.y
                );
                vi++;
                index++;
            }

            // Final cleanup for closed shape
            //            if (poly.IsClosed()) {
            //                vertices[vi - 1] = vertices[firstVi];
            //            }

            size_t offset = 0;
            for (size_t i = 0; i < sliceCount; i++) {
                triangles[offset] = (uint32_t)i;
                triangles[offset + 1] = (uint32_t)i + 1;
                triangles[offset + 2] = (uint32_t)(i + poly.size());

                triangles[offset + 3] = (uint32_t)i + 1;
                triangles[offset + 4] = (uint32_t)(i + poly.size() + 1);
                triangles[offset + 5] = (uint32_t)(i + poly.size());

                offset += 6;
            }

            result.Update(vertices, triangles, uvs);

            return result;
        }
    };
} // namespace PJ
