#include "RenderModelBuilder.h"
#include "Mesh.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "SomeRenderer.h"
#include "SomeShaderProgram.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

PJ::UVTransformFunc UVTransformFuncs::textureCoordinates = [](SomeTexture& texture,
                                                              VectorList<Vector2>& uvs) {
    std::transform(uvs.begin(), uvs.end(), uvs.begin(), [=](Vector2 uv) {
        uv *= texture.NormalSize();
        uv += texture.NormalOrigin();
        return uv;
    });
};

RenderModel RenderModelBuilder::Build(
    SomeRenderer& renderer, Mesh const& mesh, RenderMaterial& material,
    VectorList<SP<SomeTexture>> const& textures, UVTransformFunc uvTransformFunc
) {
    auto result = Build(renderer.owner, mesh, material, textures, uvTransformFunc);
    result.zIndex = renderer.model.zIndex;
    return result;
}

RenderModel RenderModelBuilder::Build(
    WorldNode* node, Mesh const& mesh, RenderMaterial& material,
    VectorList<SP<SomeTexture>> const& textures, UVTransformFunc uvTransformFunc
) {
    RenderModel renderModel(&material);
    renderModel.name = node ? node->name : "";
    renderModel.id = node ? MakeString((uint64_t)node) : "";
    renderModel.mesh = mesh;
    renderModel.matrix = node ? node->ModelMatrix() : renderModel.matrix;

    if (!IsEmpty(textures) && uvTransformFunc) {
        uvTransformFunc(*textures[0], renderModel.mesh.UVs());
    }

    return renderModel;
}
