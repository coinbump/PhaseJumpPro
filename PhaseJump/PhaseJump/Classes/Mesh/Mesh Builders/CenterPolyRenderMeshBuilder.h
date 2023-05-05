#ifndef PJCENTERPOLYMESHRENDERBUILDER_H
#define PJCENTERPOLYMESHRENDERBUILDER_H

#include "SomeCenterPolyRenderMeshBuilder.h"
#include "RenderMesh.h"

/*
 RATING: 5 stars
 Tested and works. Needs unit tests
 CODE REVIEW: 4/16/23
 */
namespace PJ
{
    /// <summary>
    /// A mesh that can be defined by a center point at (0, 0) and a surrounding path
    /// Example: capsule, rounded rectangle, arc, ellipse, circle
    /// </summary>
    class CenterPolyRenderMeshBuilder : public SomeCenterPolyRenderMeshBuilder
    {
    public:
        int MeshVertexCount() const { return (int)polygon.size() + 1; }
        int SliceCount() const { return (int)polygon.size() - 1; }

        CenterPolyRenderMeshBuilder() { }

        RenderMesh BuildRenderMesh() override
        {
            RenderMesh result;

            if (polygon.size() < 2) { return result; }

            auto vertexCount = MeshVertexCount();

            VectorList<Vector3> vertices;
            vertices.resize(vertexCount);

            VectorList<Vector2> uvs;
            uvs.resize(vertexCount);

            auto sliceCount = SliceCount();
            auto trianglesCount = sliceCount * 3;

            VectorList<uint32_t> triangles;
            triangles.resize(trianglesCount);

            // Center vertex
            vertices[0] = Vector3::zero;
            uvs[0] = Vector2(0.5f, 0.5f);

            Polygon polyWithCenter;
            polyWithCenter.Add(vertices[0]);
            polyWithCenter.AddRange(polygon);
            auto polygonMin = polyWithCenter.Min();
            auto polygonSize = polyWithCenter.Size();

            // Edge vertices
            int vi = 1;
            for (auto vertex : polygon)
            {
                vertices[vi] = vertex;
                uvs[vi] = Vector2((vertex.x - polygonMin.x) / polygonSize.x, (vertex.y - polygonMin.y) / polygonSize.y);
                vi++;
            }

            auto offset = 0;
            for (int i = 0; i < sliceCount; i++)
            {
                triangles[offset] = 0;
                triangles[offset + 1] = i + 1;
                triangles[offset + 2] = i + 2;

                offset += 3;
            }

            result.Update(std::make_optional(vertices), std::make_optional(triangles), std::make_optional(uvs));

            return result;
        }
    };
}

#endif
