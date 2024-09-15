#include "MeshRenderer.h"
#include "Angle.h"
#include "QuadMeshBuilder.h"
#include "RenderContextModel.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeShaderProgram.h"

using namespace std;
using namespace PJ;

MeshRenderer::MeshRenderer(Vector3 worldSize) :
    Base(worldSize) {
    model.SetBuildMeshFunc([](RendererModel const& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });

    model.material = MAKE<RenderMaterial>();

    auto program = SomeShaderProgram::registry["color.vary"];
    GUARD(program)

    model.material->SetShaderProgram(program);
}
