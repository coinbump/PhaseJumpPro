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
    class SomeShaderProgram;
    struct RenderModel;
    struct Mesh;
    class RenderMaterial;
    class Matrix4x4;
    class SomeTexture;
    class WorldNode;
    class SomeRenderer;
    class SomeMaterialRenderer;
    class RendererModel;

    /// Called to transform uv coordinates for a texture (usally for textures inside a texture
    /// atlas)
    using UVTransformFunc = std::function<void(SomeTexture&, VectorList<Vector2>& uvs)>;

    namespace UVTransformFuncs {
        extern UVTransformFunc textureCoordinates;
    }

    struct RenderModelBuilder {
        std::optional<RenderModel> Build(SomeMaterialRenderer& renderer, RendererModel& model);

    protected:
        std::optional<RenderModel> Build(
            WorldNode* node, Mesh const& mesh, RenderMaterial& material,
            VectorList<SP<SomeTexture>> const& textures
        );
    };
} // namespace PJ
