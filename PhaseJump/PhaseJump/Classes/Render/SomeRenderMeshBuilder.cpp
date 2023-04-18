#include "SomeRenderMeshBuilder.h"
#include "Mesh.h"
#include "RenderMesh.h"

using namespace std;
using namespace PJ;

Mesh SomeRenderMeshBuilder::BuildMesh() {
    auto renderMesh = BuildRenderMesh();
    return renderMesh.vertices;
}
