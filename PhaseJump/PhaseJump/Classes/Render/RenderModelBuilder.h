#pragma once

#include "Vector2.h"
#include "VectorList.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/17/24
 */
namespace PJ {
    class ShaderProgram;
    struct RenderModel;
    struct Mesh;
    class RenderMaterial;
    class Matrix4x4;
    class Texture;
    class WorldNode;
    class Renderer;
    class MaterialRenderer;
    class RendererModel;

    /// Called to transform uv coordinates for a texture (usally for textures inside a texture
    /// atlas)
    using UVTransformFunc = std::function<void(Texture&, VectorList<Vector2>& uvs)>;

    namespace UVTransformFuncs {
        extern UVTransformFunc textureCoordinates;
    }

    struct RenderModelBuilder {
        std::optional<RenderModel> Build(MaterialRenderer& renderer, RendererModel& model);

    protected:
        std::optional<RenderModel> Build(
            WorldNode* node, Mesh const& mesh, RenderMaterial& material,
            VectorList<SP<Texture>> const& textures
        );
    };
} // namespace PJ
