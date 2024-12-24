#include "PlaneMeshBuilder.h"

using namespace std;
using namespace PJ;

PlaneMeshBuilder::PlaneMeshBuilder(Config config) :
    meshSize(config.meshSize),
    worldSize(config.worldSize),
    faceAxis(config.faceAxis) {}

Mesh PlaneMeshBuilder::BuildMesh() {
    Mesh result;

    int vertexXCount = meshSize.x + 1;
    int vertexZCount = meshSize.y + 1;
    int verticesSize = vertexXCount * vertexZCount;

    VectorList<Vector3> vertices(verticesSize);
    VectorList<Vector2> uvs(verticesSize);

    auto cellCount = meshSize.x * meshSize.y;
    auto trianglesSize = cellCount * 6;
    VectorList<uint32_t> triangles(trianglesSize);

    for (size_t z = 0, i = 0; z < vertexZCount; z++) {
        for (size_t x = 0; x < vertexXCount; x++) {
            // Y, Z axes supported (for now)
            switch (faceAxis) {
            case Axis::Y:
                {
                    auto faceValue = worldSize.y * 0.5f * vecUp +
                                     ((float)z / (vertexZCount - 1)) * worldSize.y * vecDown;
                    vertices[i] = Vector3(
                        (float)x / (vertexXCount - 1) * worldSize.x - worldSize.x / 2.0f, faceValue,
                        0
                    );
                }
                break;
            default:
                {
                    auto faceValue = worldSize.y * 0.5f * Vector3::forward.z +
                                     (float)z / (vertexZCount - 1) * worldSize.y * Vector3::back.z;
                    vertices[i] = Vector3(
                        (float)x / (vertexXCount - 1) * worldSize.x - worldSize.x / 2.0f, 0,
                        faceValue
                    );
                }
                break;
            }

            uvs[i] =
                Vector2((float)x / (float)(vertexXCount - 1), (float)z / (float)(vertexZCount - 1));
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
