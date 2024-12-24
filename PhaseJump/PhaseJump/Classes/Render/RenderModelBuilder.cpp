#include "RenderModelBuilder.h"
#include "Mesh.h"
#include "RendererModel.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "SomeRenderer.h"
#include "SomeShaderProgram.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

PJ::UVTransformFunc UVTransformFuncs::textureCoordinates = [](SomeTexture& texture,
                                                              VectorList<Vector2>& uvs) {
    // FUTURE: support rotated textures
    std::transform(uvs.begin(), uvs.end(), uvs.begin(), [=](Vector2 uv) {
        uv *= texture.NormalSize();
        uv += texture.NormalOrigin();
        return uv;
    });
};

std::optional<RenderModel>
RenderModelBuilder::Build(SomeMaterialRenderer& renderer, RendererModel& model) {
    auto material = model.material.get();
    GUARDR_LOG(material, {}, "ERROR. Missing material")

    auto& materialTextures = material->Textures();
    auto result = Build(renderer.owner, model.Mesh(), *material, materialTextures);
    GUARDR(result, {})

    // NOTE: currently zIndex is ignored
    result->zIndex = renderer.model.zIndex;

    return result;
}

std::optional<RenderModel> RenderModelBuilder::Build(
    WorldNode* node, Mesh const& mesh, RenderMaterial& material,
    VectorList<SP<SomeTexture>> const& textures
) {
    RenderModel renderModel(&material);
    renderModel.name = node ? node->name : "";
    renderModel.id = node ? MakeString((uint64_t)node) : "";
    renderModel.mesh = mesh;
    renderModel.matrix = node ? node->ModelMatrix() : renderModel.matrix;

    return renderModel;
}
