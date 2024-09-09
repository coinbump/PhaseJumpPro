#include "RenderModelBuilder.h"
#include "Mesh.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "SomeRenderer.h"
#include "SomeShaderProgram.h"
#include "WorldNode.h"

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
    SomeRenderer& renderer, Mesh const& mesh, RenderMaterial& material,
    VectorList<SomeTexture*> const& textures, UVTransformFunc uvTransformFunc
) {
    auto result = Build(renderer.owner, mesh, material, textures, uvTransformFunc);
    result.zIndex = renderer.model.zIndex;
    return result;
}

RenderModel RenderModelBuilder::Build(
    WorldNode* node, Mesh const& mesh, RenderMaterial& material,
    VectorList<SomeTexture*> const& textures, UVTransformFunc uvTransformFunc
) {
    RenderModel renderModel(&material);
    renderModel.name = node ? node->name : "";
    renderModel.id = node ? MakeString(node->IntId()) : "";
    renderModel.mesh = mesh;
    renderModel.matrix = node ? node->ModelMatrix() : renderModel.matrix;

    for (auto& texture : textures) {
        GUARD_CONTINUE(texture)

        auto textureRenderModel = texture->MakeRenderModel();
        GUARD_CONTINUE(textureRenderModel)

        Add(renderModel.textureModels, *textureRenderModel);
    }

    if (!IsEmpty(textures) && uvTransformFunc) {
        auto& textureModel = renderModel.textureModels[0];
        uvTransformFunc(textureModel, renderModel.mesh.UVs());
    }

    return renderModel;
}

// RenderModel RenderModelBuilder::Build(
//     Mesh const& mesh, RenderMaterial& material, VectorList<SomeTexture*> const& textures,
//     Matrix4x4 modelMatrix, float z, UVTransformFunc uvTransformFunc
//) {
//     RenderModel renderModel(&material);
//     renderModel.mesh = mesh;
//     renderModel.matrix = modelMatrix;
//     renderModel.z = z;
//
//     for (auto& texture : textures) {
//         GUARD_CONTINUE(texture)
//
//         auto textureRenderModel = texture->MakeRenderModel();
//         GUARD_CONTINUE(textureRenderModel)
//
//         Add(renderModel.textureModels, *textureRenderModel);
//     }
//
//     if (!IsEmpty(textures) && uvTransformFunc) {
//         auto& textureModel = renderModel.textureModels[0];
//         uvTransformFunc(textureModel, renderModel.UVs());
//     }
//
//     return renderModel;
// }
