#include "SpriteRenderer.h"
#include "QuadMeshBuilder.h"
#include "RenderIntoModel.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeRenderEngine.h"
#include "Utils.h"

using namespace std;
using namespace PJ;

SpriteRenderer::SpriteRenderer(SP<SomeTexture> texture) :
    texture(texture) {
    material = MAKE<RenderMaterial>();
    GUARD(texture);

    material->Add(texture);

    QuadMeshBuilder builder(Vector2((float)texture->size.x, (float)texture->size.y));
    mesh = builder.BuildMesh();
}

SpriteRenderer::SpriteRenderer(SP<RenderMaterial> material) {
    this->material = material;

    GUARD(!IsEmpty(material->Textures()))

    texture = *material->Textures().begin();
    QuadMeshBuilder builder(Vector2((float)texture->size.x, (float)texture->size.y));
    mesh = builder.BuildMesh();
}

VectorList<RenderModel> SpriteRenderer::MakeRenderModels(RenderIntoModel const& model) {
    VectorList<RenderModel> result;

    GUARDR(owner, result)

    if (nullptr == material) {
        PJLog("ERROR. Missing material.");
        return result;
    }

    RenderModelBuilder builder;
    VectorList<SomeTexture*> textures{ texture.get() };
    auto renderModel = builder.Build(
        mesh, *material, textures, ModelMatrix(), owner->transform->WorldPosition().z
    );

    if (flipX || flipY) {
        std::transform(
            renderModel.UVs().cbegin(), renderModel.UVs().cend(), renderModel.UVs().begin(),
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

    result.push_back(renderModel);
    return result;
}

Vector2 SpriteRenderer::Size() const {
    if (material && texture) {
        return Vector2((float)texture->size.x, (float)texture->size.y);
    }

    return Vector2();
}

void SpriteRenderer::SetColor(Color color) {
    if (material) {
        if (IsEmpty(material->UniformColors())) {
            material->AddUniformColor(color);
        } else {
            material->SetUniformColor(0, color);
        }
    }
}
