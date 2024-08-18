#include "RenderModelBuilder.h"
#include "Mesh.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "SomeShaderProgram.h"

using namespace std;
using namespace PJ;

RenderModel RenderModelBuilder::Build(
    Mesh const& mesh, RenderMaterial& material, VectorList<SomeTexture*> const& textures,
    Matrix4x4 modelMatrix, float z
) {
    RenderModel renderModel(material);
    renderModel.mesh = mesh;
    renderModel.matrix = modelMatrix;
    renderModel.z = z;

    for (auto& texture : textures) {
        GUARD_CONTINUE(texture)

        auto textureRenderModel = texture->MakeRenderModel();
        GUARD_CONTINUE(textureRenderModel)

        renderModel.textureModels.Add(*textureRenderModel);
    }

    return renderModel;
}
