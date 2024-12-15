#include "ArcMeshBuilder.h"

using namespace std;
using namespace PJ;

Mesh ArcMeshBuilder::BuildMesh() {
    CenterPolyMeshBuilder builder(worldSize, CenterPolyModel(startAngle, angleDelta, angleStep));
    return builder.BuildMesh();
}
