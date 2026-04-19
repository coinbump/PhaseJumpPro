#include "PolygonMeshBuilder.h"
#include <algorithm>

using namespace std;
using namespace PJ;

Mesh PolygonMeshBuilder::BuildMesh() {
    Mesh result;

    // Normalize UVs against the polygon's own bounding box. Guard against
    // degenerate polygons whose width or height is zero or negative — they
    // would produce divide-by-zero UVs.
    Vector2 const polygonMin = polygon.Min();
    Vector2 const polygonSize = polygon.Size();
    GUARDR(polygonSize.x > 0 && polygonSize.y > 0, result)

    auto triangles = polygon.BuildTriangles();
    GUARDR(!triangles.empty(), result)

    VectorList<Vector3> vertices;
    VectorList<Vector2> uvs;
    VectorList<uint32_t> indices;

    vertices.reserve(triangles.size() * 3);
    uvs.reserve(triangles.size() * 3);
    indices.reserve(triangles.size() * 3);

    // Share vertices
    auto findOrAdd = [&](Vector2 v) -> uint32_t {
        for (size_t i = 0; i < vertices.size(); i++) {
            auto const& existing = vertices[i];
            if (existing.x == v.x && existing.y == v.y) {
                return (uint32_t)i;
            }
        }
        vertices.push_back(Vector3(v.x, v.y, 0.0f));

        uvs.push_back(
            Vector2((v.x - polygonMin.x) / polygonSize.x, (v.y - polygonMin.y) / polygonSize.y)
        );
        return (uint32_t)(vertices.size() - 1);
    };

    for (auto const& tri : triangles) {
        Vector2 a = tri.value[0];
        Vector2 b = tri.value[1];
        Vector2 c = tri.value[2];

        // Force clockwise winding. For standard Y-up coordinates, a
        // counter-clockwise triangle has a positive signed area. If the
        // triangle came out CCW, swap two vertices to flip it to CW.
        //
        //   signedArea = (b - a) x (c - a) / 2
        //
        // The constant 1/2 is omitted since we only need the sign.
        float const signedArea = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
        if (signedArea > 0.0f) {
            std::swap(b, c);
        }

        indices.push_back(findOrAdd(a));
        indices.push_back(findOrAdd(b));
        indices.push_back(findOrAdd(c));
    }

    result.Update(vertices, indices, uvs);
    return result;
}
