#include "RoundCornersMeshBuilder.h"
#include "CenterPolyMeshBuilder.h"
#include "QuadMeshBuilder.h"

using namespace std;
using namespace PJ;

RoundCornersMeshBuilder::RoundCornersMeshBuilder(Config config) :
    corners(config.corners),
    worldSize(config.worldSize),
    angleStep(config.angleStep) {}

Mesh RoundCornersMeshBuilder::BuildMesh() {
    GUARDR(angleStep.Degrees() > 0, {})

    Mesh result;

    auto cornerLeftSize = std::max(corners.topLeft.x, corners.bottomLeft.x);
    auto cornerRightSize = std::max(corners.topRight.x, corners.bottomRight.x);
    auto cornerTopSize = std::max(corners.topLeft.y, corners.topRight.y);
    auto cornerBottomSize = std::max(corners.bottomLeft.y, corners.bottomRight.y);

    Vector2 centerSize{ worldSize.x - cornerLeftSize - cornerRightSize,
                        worldSize.y - cornerTopSize - cornerBottomSize };
    Vector2 leftSize{ cornerLeftSize, centerSize.y };
    Vector2 rightSize{ cornerRightSize, centerSize.y };
    Vector2 topSize{ centerSize.x, cornerTopSize };
    Vector2 bottomSize{ centerSize.x, cornerBottomSize };

    result += QuadMeshBuilder(centerSize).BuildMesh();
    result += QuadMeshBuilder(leftSize).BuildMesh().Offset(Vector2{
        (centerSize.x / 2.0f + leftSize.x / 2.0f) * vecLeft, 0 });
    result += QuadMeshBuilder(rightSize).BuildMesh().Offset(Vector2{
        (centerSize.x / 2.0f + rightSize.x / 2.0f) * vecRight, 0 });
    result += QuadMeshBuilder(topSize).BuildMesh().Offset(Vector2{
        0, (centerSize.y / 2.0f + topSize.y / 2.0f) * vecUp });
    result += QuadMeshBuilder(bottomSize)
                  .BuildMesh()
                  .Offset(Vector2{ 0, (centerSize.y / 2.0f + bottomSize.y / 2.0f) * vecDown });

    auto model = CenterPolyModel::ellipse;
    model.angleStep = angleStep;
    model.angleDelta = Angle::WithDegrees(90);

    if (corners.topLeft.x > 0 && corners.topLeft.y > 0) {
        model.startAngle = Angle::WithDegrees(-90);
        CenterPolyMeshBuilder builder(corners.topLeft * 2.0f, model);
        result += builder.BuildMesh().Offset(Vector2{ (centerSize.x / 2.0f) * vecLeft,
                                                      (centerSize.y / 2.0f) * vecUp });
    }

    if (corners.topRight.x > 0 && corners.topRight.y > 0) {
        model.startAngle = Angle::WithDegrees(0);
        CenterPolyMeshBuilder builder(corners.topRight * 2.0f, model);
        result += builder.BuildMesh().Offset(Vector2{ (centerSize.x / 2.0f) * vecRight,
                                                      (centerSize.y / 2.0f) * vecUp });
    }

    if (corners.bottomLeft.x > 0 && corners.bottomLeft.y > 0) {
        model.startAngle = Angle::WithDegrees(180);
        CenterPolyMeshBuilder builder(corners.bottomLeft * 2.0f, model);
        result += builder.BuildMesh().Offset(Vector2{ (centerSize.x / 2.0f) * vecLeft,
                                                      (centerSize.y / 2.0f) * vecDown });
    }

    if (corners.bottomRight.x > 0 && corners.bottomRight.y > 0) {
        model.startAngle = Angle::WithDegrees(90);
        CenterPolyMeshBuilder builder(corners.bottomRight * 2.0f, model);
        result += builder.BuildMesh().Offset(Vector2{ (centerSize.x / 2.0f) * vecRight,
                                                      (centerSize.y / 2.0f) * vecDown });
    }

    return result;
}
