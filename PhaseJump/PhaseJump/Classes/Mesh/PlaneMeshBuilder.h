#pragma once

#include "Axis.h"
#include "SomeMeshBuilder.h"
#include "Vector2.h"
#include "Vector3.h"

/*
 RATING: 4 stars
 Tested and works. Needs unit tests (Has unit tests in C# equivalent)
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    /// A plane mesh of NxN cells built across the x,y, or z axis
    class PlaneMeshBuilder : public SomeMeshBuilder {
    public:
        Vector2Int meshSize{ 3, 3 };
        Vector2 worldSize{ 10.0f, 10.0f };
        Axis faceAxis = Axis::Z;

        Vector2Int VerticesSize() const {
            int vertexXCount = meshSize.x + 1;
            int vertexZCount = meshSize.y + 1;
            return Vector2Int(vertexXCount, vertexZCount);
        }

        size_t MeshVertexCount() const {
            return VerticesSize().x * VerticesSize().y;
        }

        PlaneMeshBuilder() {}

        PlaneMeshBuilder(
            Vector2 worldSize = Vector2::one, Vector2Int meshSize = Vector2Int::three,
            Axis faceAxis = Axis::Z
        ) :
            meshSize(meshSize),
            worldSize(worldSize),
            faceAxis(faceAxis) {}

        Mesh BuildMesh() override {
            Mesh result;

            int vertexXCount = meshSize.x + 1;
            int vertexZCount = meshSize.y + 1;
            int verticesSize = vertexXCount * vertexZCount;

            VectorList<Vector3> vertices(verticesSize, Vector3{});

            auto cellCount = meshSize.x * meshSize.y;
            auto trianglesSize = cellCount * 6;
            VectorList<uint32_t> triangles(trianglesSize, 0);
            auto uvSize = verticesSize;
            VectorList<Vector2> uvs(uvSize, Vector2{});

            for (size_t z = 0, i = 0; z < vertexZCount; z++) {
                for (size_t x = 0; x < vertexXCount; x++) {
                    // Y, Z axes supported (for now)
                    switch (faceAxis) {
                    case Axis::Y:
                        {
                            auto faceValue =
                                worldSize.y * 0.5f * vecUp +
                                ((float)z / (vertexZCount - 1)) * worldSize.y * vecDown;
                            vertices[i] = Vector3(
                                (float)x / (vertexXCount - 1) * worldSize.x - worldSize.x / 2.0f,
                                faceValue, 0
                            );
                        }
                        break;
                    default:
                        {
                            auto faceValue =
                                worldSize.y * 0.5f * Vector3::forward.z +
                                (float)z / (vertexZCount - 1) * worldSize.y * Vector3::back.z;
                            vertices[i] = Vector3(
                                (float)x / (vertexXCount - 1) * worldSize.x - worldSize.x / 2.0f, 0,
                                faceValue
                            );
                        }
                        break;
                    }

                    uvs[i] = Vector2(
                        (float)x / (float)(vertexXCount - 1), (float)z / (float)(vertexZCount - 1)
                    );
                    i++;
                }
            }

            for (int z = 0, i = 0; z < meshSize.y; z++) {
                for (int x = 0; x < meshSize.x; x++) {
                    auto c = z * vertexXCount + x;

                    triangles[i] = c;
                    triangles[i + 1] = c + 1;
                    triangles[i + 2] = c + vertexXCount;
                    triangles[i + 3] = c + 1;
                    triangles[i + 4] = c + vertexXCount + 1;
                    triangles[i + 5] = c + vertexXCount;
                    i += 6;
                }
            }

            result.Update(vertices, triangles, uvs);

            return result;
        }
    };
} // namespace PJ
