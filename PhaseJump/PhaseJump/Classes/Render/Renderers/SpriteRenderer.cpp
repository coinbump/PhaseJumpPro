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

    auto program = SomeShaderProgram::registry.find("texture.vary");
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

VectorList<RenderModel> SpriteRenderer::MakeRenderModels() {
    VectorList<RenderModel> result;

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
    auto renderModel = builder.Build(
        mesh, *material, textures, ModelMatrix(), 0
    ); // TODO: owner->transform.WorldPosition().z);

    renderModel.colors = VectorList<RenderColor>(mesh.vertices.size(), color);

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
    this->color = color;
}
