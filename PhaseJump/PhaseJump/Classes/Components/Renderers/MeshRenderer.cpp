#include "MeshRenderer.h"
#include "Angle.h"
#include "MaterialRenderModel.h"
#include "QuadMeshBuilder.h"
#include "RenderContextModel.h"
#include "RenderMaterial.h"
#include "ShaderProgram.h"
#include "UIPlanner.h"

using namespace std;
using namespace PJ;

MeshRenderer::MeshRenderer(Vector3 worldSize) :
    core(this, worldSize) {
    core.model.SetBuildMeshFunc([](RendererModel const& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });

    core.model.material =
        MAKE<RenderMaterial>(RenderMaterial::Config{ .shaderId = ShaderId::ColorVary });

    Override(planUIFuncs[UIContextId::Inspector], core.MakePlanUIFunc());
}
