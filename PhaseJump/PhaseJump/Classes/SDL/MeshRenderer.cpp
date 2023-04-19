#include "MeshRenderer.h"
#include "RenderIntoModel.h"
#include "RenderModel.h"
#include "SomeRenderEngine.h"
#include "Angle.h"

using namespace std;
using namespace PJ;

void MeshRenderer::RenderInto(RenderIntoModel model) {
    if (nullptr == material || nullptr == material->shaderProgram) {
        PJLog("ERROR. Missing material.");
        return;
    }

    RenderModel renderModel(*material->shaderProgram);
    renderModel.vertices = mesh.vertices;
    renderModel.indices = mesh.triangles;
    renderModel.colors = material->colors;
    renderModel.uniformColors = material->uniformColors;

    auto owner = this->owner.lock();
    Matrix4x4 translateMatrix, scaleMatrix, rotationMatrix;
    translateMatrix.LoadTranslate(owner->transform->position);
    scaleMatrix.LoadScale(owner->transform->scale);

    // This is 2D rotation only
    // FUTURE: support 3D rotation if needed.
    rotationMatrix.LoadZRadRotation(Angle::DegreesAngle(owner->transform->rotation.z).Radians());

    auto m1 = translateMatrix * rotationMatrix;
    renderModel.matrix = m1 * scaleMatrix;

    model.renderContext->renderEngine->Render(renderModel);
}
