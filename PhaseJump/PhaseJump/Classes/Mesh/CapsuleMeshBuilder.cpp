#include "CapsuleMeshBuilder.h"
#include "CenterPolyMeshBuilder.h"
#include "QuadMeshBuilder.h"

using namespace std;
using namespace PJ;

CapsuleMeshBuilder::CapsuleMeshBuilder(Config const& config) :
    axis(config.axis),
    worldSize(config.worldSize),
    angleStep(config.angleStep) {}

Mesh CapsuleMeshBuilder::BuildMesh() {
    GUARDR(worldSize.x > 0 && worldSize.y > 0, {})
    GUARDR(angleStep.Degrees() > 0, {})

    Mesh result;

    float startAngleStart = 180;
    float endAngleStart = 0;

    switch (axis) {
    case Axis2D::Y:
        startAngleStart = -90;
        endAngleStart = 90;
        break;
    case Axis2D::X:
        break;
    }

    float capRadius = CapRadius();

    Vector2 frameSize = worldSize;
    auto frameAxisLength = FrameAxisLength();
    frameSize.AxisValue(axis) = frameAxisLength;
    QuadMeshBuilder frameBuilder(frameSize);
    auto frameMesh = frameBuilder.BuildMesh();

    Vector2 centerPolySize{ capRadius * 2.0f, capRadius * 2.0f };

    CenterPolyMeshBuilder startCapBuilder(
        centerPolySize, { .startAngle = Angle::WithDegrees(startAngleStart),
                          .angleStep = angleStep,
                          .angleDelta = Angle::WithDegrees(180) }
    );
    auto startCapMesh = startCapBuilder.BuildMesh();
    Vector2 startCapOffset;
    startCapOffset.AxisValue(axis) = frameAxisLength / 2.0f;
    startCapOffset *= Vector2::upLeft;
    startCapMesh.Offset(startCapOffset);

    CenterPolyMeshBuilder endCapBuilder(
        centerPolySize, { .startAngle = Angle::WithDegrees(endAngleStart),
                          .angleStep = angleStep,
                          .angleDelta = Angle::WithDegrees(180) }
    );
    auto endCapMesh = endCapBuilder.BuildMesh();
    Vector2 endCapOffset;
    endCapOffset.AxisValue(axis) = frameAxisLength / 2.0f;
    endCapOffset *= Vector2::downRight;
    endCapMesh.Offset(endCapOffset);

    return startCapMesh + frameMesh + endCapMesh;
}
