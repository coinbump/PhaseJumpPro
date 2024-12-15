#include "PolyFrameMeshBuilder.h"
#include "CenterPolyMeshBuilder.h"

using namespace std;
using namespace PJ;

PolyFrameMeshBuilder::PolyFrameMeshBuilder(Config config) :
    poly(config.poly),
    polyClose(config.polyClose),
    strokeWidth(config.strokeWidth),
    startCap(config.startCap),
    endCap(config.endCap),
    corner(config.corner) {}

Mesh PolyFrameMeshBuilder::BuildMesh() {
    Mesh result;
    GUARDR(poly.Count() >= 2, {});

    bool isClosed = polyClose == PolyClose::Closed;
    auto segmentCount = isClosed ? poly.Count() : poly.Count() - 1;
    auto cornersCount = segmentCount - 1;

    // Each segment of a poly must be its own quad, in order for the stroke width to
    // Render at the proper offset along that segment.
    auto vertexCount = segmentCount * 4;

    GUARDR(segmentCount > 0 && vertexCount > 0, {})

    VectorList<Vector3> vertices(vertexCount);
    auto trianglesSize = segmentCount * 6;
    VectorList<uint32_t> triangles(trianglesSize);
    auto uvSize = vertexCount;
    VectorList<Vector2> uvs(uvSize);

    // Outer frame
    auto meshVertexIndex = 0;
    for (size_t i = 0; i < segmentCount; i++) {
        auto vertex = poly[i];
        auto nextVertex = poly[(i + 1) % poly.Count()];

        Vector2 angleVector(nextVertex.x - vertex.x, nextVertex.y - vertex.y);
        // Debug.Log("Angle Vector: " + angleVector.ToString());
        Angle degreeAngle(angleVector);
        // Debug.Log("Degree Angle: " + degreeAngle.ToString());

        auto outerOffset =
            Angle::WithDegrees(degreeAngle.Degrees() - 90.0f).ToVector2(strokeWidth / 2.0f);
        auto innerOffset =
            Angle::WithDegrees(degreeAngle.Degrees() + 90.0f).ToVector2(strokeWidth / 2.0f);

        // Debug.Log("Outer Offset: " + outerOffset.ToString());
        // Debug.Log("Inner Offset: " + innerOffset.ToString());

        vertices[meshVertexIndex + 0] = vertex + Vector3(outerOffset.x, outerOffset.y, 0);
        vertices[meshVertexIndex + 1] = nextVertex + Vector3(outerOffset.x, outerOffset.y, 0);
        vertices[meshVertexIndex + 2] = vertex + Vector3(innerOffset.x, innerOffset.y, 0);
        vertices[meshVertexIndex + 3] = nextVertex + Vector3(innerOffset.x, innerOffset.y, 0);

        meshVertexIndex += 4;
    }

    auto center = poly.Center();
    auto size = poly.Size();
    if (size.x > 0 && size.y > 0) {
        for (size_t i = 0; i < uvSize; i++) {
            auto vertex = vertices[i];
            auto distanceFromCenter = vertex - center;
            Vector2 ratio(
                (distanceFromCenter.x + size.x / 2.0f) / size.x,
                (distanceFromCenter.y + size.y / 2.0f) / size.y
            );
            uvs[i] = ratio;
        }
    } else {
        for (size_t i = 0; i < uvSize; i++) {
            uvs[i] = {};
        }
    }

    auto t = 0;
    meshVertexIndex = 0;
    for (size_t i = 0; i < segmentCount; i++) {
        triangles[t] = meshVertexIndex;
        triangles[t + 1] = meshVertexIndex + 1;
        triangles[t + 2] = meshVertexIndex + 2;
        triangles[t + 3] = meshVertexIndex + 1;
        triangles[t + 4] = meshVertexIndex + 3;
        triangles[t + 5] = meshVertexIndex + 2;

        meshVertexIndex += 4;
        t += 6;
    }

    result.Update(vertices, triangles, uvs);

    Vector2 strokeSize{ strokeWidth, strokeWidth };

    if (!isClosed) {
        if (PathCap::Round == startCap) {
            Vector2 segmentPoint1 = vertices[0];
            Vector2 segmentPoint2 = vertices[1];
            Vector2 pointDelta = segmentPoint1 - segmentPoint2;
            Angle segmentAngle(pointDelta);
            auto polyModel = CenterPolyModel::ellipse;
            polyModel.startAngle = Angle::WithDegrees(segmentAngle.Degrees() - 90);
            polyModel.angleDelta = Angle::WithDegrees(180);

            CenterPolyMeshBuilder capBuilder(strokeSize, polyModel);
            auto capMesh = capBuilder.BuildMesh();
            Vector2 midPoint = Lerp(vertices[0], vertices[2], 0.5f);
            capMesh.OffsetBy(midPoint);

            result += capMesh;
        }

        if (PathCap::Round == endCap) {
            Vector2 segmentPoint1 = vertices[vertices.size() - 4];
            Vector2 segmentPoint2 = vertices[vertices.size() - 3];
            Vector2 pointDelta = segmentPoint2 - segmentPoint1;
            Angle segmentAngle(pointDelta);
            auto polyModel = CenterPolyModel::ellipse;
            polyModel.startAngle = Angle::WithDegrees(segmentAngle.Degrees() - 90);
            polyModel.angleDelta = Angle::WithDegrees(180);

            CenterPolyMeshBuilder capBuilder(strokeSize, polyModel);
            auto capMesh = capBuilder.BuildMesh();
            Vector2 midPoint =
                Lerp(vertices[vertices.size() - 3], vertices[vertices.size() - 1], 0.5f);
            capMesh.OffsetBy(midPoint);

            result += capMesh;
        }
    }

    switch (corner) {
    case PathCorner::None:
        break;
    case PathCorner::Round:
        {
            int vi = 0;
            for (int corner = 0; corner < cornersCount && vi < vertices.size() - 4; corner++) {
                Vector2 segmentPoint1 = vertices[vi];
                Vector2 segmentPoint2 = vertices[vi + 1];
                Vector2 pointDelta = segmentPoint2 - segmentPoint1;
                Angle segmentAngle(pointDelta);

                // FUTURE: optimize by creating partial arcs instead of circles, but this adds new
                // edge cases to test depending on line direction
                //                Vector2 nextSegmentPoint1 = vertices[vi + 4];
                //                Vector2 nextSegmentPoint2 = vertices[vi + 5];
                //                Vector2 nextPointDelta = nextSegmentPoint2 - nextSegmentPoint1;
                //                Angle nextSegmentAngle(nextPointDelta);
                //
                //                Angle startSegmentAngle = segmentAngle;
                //                Angle endSegmentAngle = nextSegmentAngle;
                //
                //                if (endSegmentAngle.Degrees() < startSegmentAngle.Degrees()) {
                //                    std::swap(endSegmentAngle, startSegmentAngle);
                //                }
                //
                //                auto polyModel = CenterPolyModel::ellipse;
                //                polyModel.startAngle = startSegmentAngle;
                //                polyModel.angleDelta = endSegmentAngle - polyModel.startAngle;

                auto polyModel = CenterPolyModel::ellipse;
                CenterPolyMeshBuilder capBuilder(strokeSize, polyModel);
                auto capMesh = capBuilder.BuildMesh();
                Vector2 midPoint = Lerp(vertices[vi + 1], vertices[vi + 3], 0.5f);
                capMesh.OffsetBy(midPoint);

                result += capMesh;
                vi += 4;
            }
            break;
        }
    case PathCorner::Bevel:
        Log("ERROR. Unsupported path corner");
        break;
    case PathCorner::Miter:
        Log("ERROR. Unsupported path corner");
        break;
    }

    return result;
}
