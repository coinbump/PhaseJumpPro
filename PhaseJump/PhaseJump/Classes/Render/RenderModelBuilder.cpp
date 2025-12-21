#include "RenderModelBuilder.h"
#include "Mesh.h"
#include "Renderer.h"
#include "RendererModel.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "ShaderProgram.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

PJ::UVTransformFunc UVTransformFuncs::textureCoordinates = [](Texture& texture,
                                                              VectorList<Vector2>& uvs) {
    // FUTURE: support rotated textures
    std::transform(uvs.begin(), uvs.end(), uvs.begin(), [=](Vector2 uv) {
        uv *= texture.NormalSize();
        uv += texture.NormalOrigin();
        return uv;
    });
};

std::optional<RenderModel>
RenderModelBuilder::Build(MaterialRenderer& renderer, RendererModel& model) {
    auto material = model.material.get();
    GUARDR_LOG(material, {}, "ERROR. Missing material")

    auto& materialTextures = material->Textures();
    auto result = Build(renderer.owner, model.GetMesh(), *material, materialTextures);
    GUARDR(result, {})

    // NOTE: currently zIndex is ignored
    result->zIndex = renderer.model.zIndex;

    return result;
}

std::optional<RenderModel> RenderModelBuilder::Build(
    WorldNode* node, Mesh const& mesh, RenderMaterial& material,
    VectorList<SP<Texture>> const& textures
) {
    RenderModel renderModel(&material);
    renderModel.name = node ? node->Name() : "";
    renderModel.id = node ? MakeString((uint64_t)node) : "";

    renderModel.SetMeshPtr(&mesh);

    renderModel.matrix = node ? node->ModelMatrix() : renderModel.matrix;

    return renderModel;
}
