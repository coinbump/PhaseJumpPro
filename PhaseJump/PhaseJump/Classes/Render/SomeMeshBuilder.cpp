#include "SomeMeshBuilder.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "VertexList.h"

using namespace std;
using namespace PJ;

VertexList SomeMeshBuilder::BuildVertexList() {
    auto renderMesh = BuildMesh();
    return renderMesh.vertices;
}

void SomeMeshBuilder::Awake() {
    Base::Awake();

    auto meshRenderer = GetComponent<MeshRenderer>();
    if (meshRenderer) {
        meshRenderer->mesh = BuildMesh();
    }
}
