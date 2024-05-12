#include "TiledMeshBuilder.h"

using namespace std;
using namespace PJ;

UVRect const UVRect::one(Vector2(0, 0), Vector2(1, 1));

Vector2Int TiledMeshBuilder::MeshSize() const {
    int numTilesX = (int)floor(worldSize.x / tileSize.x);
    if (fmod(worldSize.x, tileSize.x)) {
        numTilesX++;
    }

    int numTilesY = (int)floor(worldSize.y / tileSize.y);
    if (fmod(worldSize.y, tileSize.y)) {
        numTilesY++;
    }

    return Vector2Int(numTilesX, numTilesY);
} // Tested

Mesh TiledMeshBuilder::BuildMesh()
{
    Mesh result;

    auto meshSize = MeshSize();

    auto cellCount = meshSize.x * meshSize.y;
    auto trianglesSize = cellCount * 6;
    VectorList<uint32_t> triangles;
    triangles.resize(trianglesSize);

    int vertexXCount = meshSize.x * 2;
    int vertexYCount = meshSize.y * 2;
    int verticesSize = vertexXCount * vertexYCount;

    VectorList<Vector3> vertices;
    vertices.resize(verticesSize);
    auto uvSize = verticesSize;
    VectorList<Vector2> uvs;
    uvs.resize(uvSize);

    for (int meshY = 0, i = 0; meshY < meshSize.y; meshY++) {
        for (int y = 0; y < 2; y++) {
            for (int meshX = 0; meshX < meshSize.x; meshX++) {
                auto yThis = meshY + y;

                float yTarget = yThis * tileSize.y;
                float yPos = std::min(yTarget, worldSize.y);
                float yUnderflow = yPos - yTarget;
                float uvY = y == 0 ? 1 : 1.0f - ((tileSize.y + yUnderflow) / tileSize.y);
                yPos = worldSize.y - yPos - worldSize.y / 2.0f;

                for (int x = 0; x < 2; x++) {
                    auto xThis = meshX + x;
                    float xTarget = xThis * tileSize.x;
                    float xPos = std::min(xTarget, worldSize.x);
                    float xUnderflow = xPos - xTarget;
                    float uvX = x == 0 ? 0 : (tileSize.x + xUnderflow) / tileSize.x;

                    xPos -= worldSize.x / 2.0f;

                    vertices[i] = Vector3(xPos, yPos, 0);

                    auto uvFull = Vector2(uvX, uvY);
                    Vector2 uvInBounds = uvFull * Vector2(uvBounds.size.x, uvBounds.size.y);
                    uvInBounds += Vector2(uvBounds.origin.x, uvBounds.origin.y);
                    uvs[i] = uvInBounds;
                    i++;
                }
            }
        }
    }

    for (int meshY = 0, i = 0; meshY < meshSize.y; meshY++) {
        for (int meshX = 0; meshX < meshSize.x; meshX++) {
            auto c = meshY * (vertexXCount * 2) + (meshX * 2);

            triangles[i] = c;
            triangles[i + 1] = c + 1;
            triangles[i + 2] = c + vertexXCount;
            triangles[i + 3] = c + 1;
            triangles[i + 4] = c + vertexXCount + 1;
            triangles[i + 5] = c + vertexXCount;
            i += 6;
        }
    }

    result.Update(std::make_optional(vertices), std::make_optional(triangles), std::make_optional(uvs));

    return result;
}
