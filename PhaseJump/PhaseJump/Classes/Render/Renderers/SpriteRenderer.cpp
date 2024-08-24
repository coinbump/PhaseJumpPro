#include "SpriteRenderer.h"
#include "QuadMeshBuilder.h"
#include "RenderContextModel.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeRenderEngine.h"
#include "SomeShaderProgram.h"
#include "Utils.h"

using namespace std;
using namespace PJ;

SpriteRenderer::SpriteRenderer(SP<SomeTexture> texture) :
    texture(texture) {
    material = MAKE<RenderMaterial>();
    GUARD(texture);

    material->Add(texture);

    // TODO: use texture.vary for better batching
    auto program = SomeShaderProgram::registry.find("texture.uniform");
    GUARD(program != SomeShaderProgram::registry.end())
    material->SetShaderProgram(program->second);

    QuadMeshBuilder builder(Vector2((float)texture->size.x, (float)texture->size.y));
    mesh = builder.BuildMesh();
}

SpriteRenderer::SpriteRenderer(SP<RenderMaterial> material) {
    this->material = material;
    GUARD(material)

    GUARD(!IsEmpty(material->Textures()))

    texture = *material->Textures().begin();
    QuadMeshBuilder builder(Vector2((float)texture->size.x, (float)texture->size.y));
    mesh = builder.BuildMesh();
}

VectorList<RenderModel> SpriteRenderer::MakeRenderModels(RenderContextModel const& model) {
    VectorList<RenderModel> result;

    GUARDR(owner, result)

    if (nullptr == material) {
        PJLog("ERROR. Missing material.");
        return result;
    }

    Mesh mesh = this->mesh;

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
    auto renderModel =
        builder.Build(mesh, *material, textures, ModelMatrix(), owner->transform.WorldPosition().z);

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
