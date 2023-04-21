#include "MeshRenderer.h"
#include "RenderIntoModel.h"
#include "RenderModel.h"
#include "SomeRenderEngine.h"
#include "Angle.h"
#include "RenderModelBuilder.h"

using namespace std;
using namespace PJ;

void MeshRenderer::RenderInto(RenderIntoModel model) {
    if (nullptr == material || nullptr == material->shaderProgram) {
        PJLog("ERROR. Missing material.");
        return;
    }

    RenderModelBuilder builder;
    auto renderModel = builder.Build(*material->shaderProgram, mesh, *material);
    renderModel.matrix = model.modelMatrix;

    model.renderContext->renderEngine->RenderProcess(renderModel);
}
