#pragma once

#include "Vector2.h"
#include "VectorList.h"
#include <memory>

// CODE REVIEW: ?/23
namespace PJ {
    class SomeShaderProgram;
    struct RenderModel;
    struct Mesh;
    class RenderMaterial;
    class Matrix4x4;
    class SomeTexture;
    class TextureRenderModel;

    using UVTransformFunc = std::function<void(TextureRenderModel, VectorList<Vector2>& uvs)>;

    namespace UVTransformFuncs {
        extern UVTransformFunc textureCoordinates;
    }

    struct RenderModelBuilder {
        RenderModel Build(
            Mesh const& mesh, RenderMaterial& material, VectorList<SomeTexture*> const& textures,
            Matrix4x4 modelMatrix, float z,
            UVTransformFunc uvTransformFunc = UVTransformFuncs::textureCoordinates
        );
    };
} // namespace PJ
