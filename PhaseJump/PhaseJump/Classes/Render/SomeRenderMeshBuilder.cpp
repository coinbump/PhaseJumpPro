#include "SomeRenderMeshBuilder.h"
#include "Mesh.h"
#include "RenderMesh.h"
#include "MeshRenderer.h"

using namespace std;
using namespace PJ;

Mesh SomeRenderMeshBuilder::BuildMesh() {
    auto renderMesh = BuildRenderMesh();
    return renderMesh.vertices;
}

void SomeRenderMeshBuilder::Awake() {
    Base::Awake();

    auto meshRenderer = GetComponent<MeshRenderer>();
    if (meshRenderer) {
        meshRenderer->mesh = BuildRenderMesh();
    }
}
