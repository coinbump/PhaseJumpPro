#include "SpriteRenderer.h"
#include "QuadMeshBuilder.h"
#include "RenderContextModel.h"
#include "RenderFeature.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeRenderEngine.h"
#include "SomeShaderProgram.h"
#include "Utils.h"

using namespace std;
using namespace PJ;

SpriteRenderer::SpriteRenderer(SP<SomeTexture> texture) :
    Base(texture ? Vector3(texture->size.x, texture->size.y, 0) : Vector3::zero),
    texture(texture) {
    model.material = MAKE<RenderMaterial>();
    GUARD(texture);

    model.SetMeshBuilderFunc([](RendererModel const& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });

    model.material->Add(texture);

    auto program = SomeShaderProgram::registry.find("texture.vary");
    GUARD(program != SomeShaderProgram::registry.end())
    model.material->SetShaderProgram(program->second);
    model.material->EnableFeature(RenderFeature::Blend, true);
    model.SetWorldSize(Vector2(texture->size.x, texture->size.y));
}

SpriteRenderer::SpriteRenderer(SP<RenderMaterial> material) :
    Base(vec2Zero) {
    model.material = material;
    GUARD(material)

    model.SetMeshBuilderFunc([](RendererModel const& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });

    GUARD(!IsEmpty(material->Textures()))

    texture = *material->Textures().begin();
    model.SetWorldSize(Vector2(texture->size.x, texture->size.y));
}

VectorList<RenderModel> SpriteRenderer::MakeRenderModels() {
    VectorList<RenderModel> result;

    auto material = model.material;
    if (nullptr == material) {
        PJLog("ERROR. Missing material.");
        return result;
    }

    Mesh mesh = model.Mesh();

    if (flipX || flipY) {
        std::transform(
            mesh.UVs().cbegin(), mesh.UVs().cend(), mesh.UVs().begin(),
            [this](Vector2 uv) {
                if (flipX) {
                    uv.x = 1.0f - uv.x;
                }

                if (flipY) {
                    uv.y = 1.0f - uv.y;
                }
                return uv;
            }
        );
    }

    RenderModelBuilder builder;
    VectorList<SomeTexture*> textures{ texture.get() };
    auto renderModel = builder.Build(*this, mesh, *material, textures);

    renderModel.colors = VectorList<RenderColor>(mesh.Vertices().size(), color);

    result.push_back(renderModel);
    return result;
}

Vector2 SpriteRenderer::Size() const {
    GUARDR(texture, {})
    return Vector2((float)texture->size.x, (float)texture->size.y);
}

void SpriteRenderer::SetColor(Color color) {
    this->color = color;
}
