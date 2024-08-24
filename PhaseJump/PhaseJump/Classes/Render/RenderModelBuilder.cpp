#include "RenderModelBuilder.h"
#include "Mesh.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "SomeShaderProgram.h"

using namespace std;
using namespace PJ;

PJ::UVTransformFunc UVTransformFuncs::textureCoordinates = [](TextureRenderModel textureModel,
                                                              VectorList<Vector2>& uvs) {
    std::transform(uvs.begin(), uvs.end(), uvs.begin(), [=](Vector2 uv) {
        uv *= textureModel.size;
        uv += textureModel.origin;
        return uv;
    });
};

RenderModel RenderModelBuilder::Build(
    Mesh const& mesh, RenderMaterial& material, VectorList<SomeTexture*> const& textures,
    Matrix4x4 modelMatrix, float z, UVTransformFunc uvTransformFunc
) {
    RenderModel renderModel(&material);
    renderModel.mesh = mesh;
    renderModel.matrix = modelMatrix;
    renderModel.z = z;

    for (auto& texture : textures) {
        GUARD_CONTINUE(texture)

        auto textureRenderModel = texture->MakeRenderModel();
        GUARD_CONTINUE(textureRenderModel)

        Add(renderModel.textureModels, *textureRenderModel);
    }

    if (!IsEmpty(textures) && uvTransformFunc) {
        auto& textureModel = renderModel.textureModels[0];
        uvTransformFunc(textureModel, renderModel.UVs());
    }

    return renderModel;
}
