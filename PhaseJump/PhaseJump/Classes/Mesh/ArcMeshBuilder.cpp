#include "ArcMeshBuilder.h"

using namespace std;
using namespace PJ;

size_t ArcMeshBuilder::SliceCount() const {
    if (0 == angleStep.Degrees()) {
        // Avoid divide-by-zero
        return 1;
    }

    return (size_t
    )std::ceil((endAngle.Degrees() - startAngle.Degrees()) / std::abs(angleStep.Degrees()));
}

Mesh ArcMeshBuilder::BuildMesh() {
    CenterPolyMeshBuilder builder(worldSize, CenterPolyShape(startAngle, endAngle, angleStep));
    return builder.BuildMesh();
}
