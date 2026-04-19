#include "ArcMeshBuilder.h"

using namespace std;
using namespace PJ;

ArcMeshBuilder::ArcMeshBuilder(Config const& config) :
    startAngle(config.startAngle),
    angleDelta(config.angleDelta),
    angleStep(config.angleStep),
    worldSize(config.worldSize) {}

Mesh ArcMeshBuilder::BuildMesh() {
    CenterPolyMeshBuilder builder(worldSize, CenterPolyModel(startAngle, angleDelta, angleStep));
    return builder.BuildMesh();
}
