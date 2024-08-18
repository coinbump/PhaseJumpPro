#pragma once

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

    struct RenderModelBuilder {
        RenderModel Build(
            Mesh const& mesh, RenderMaterial& material, VectorList<SomeTexture*> const& textures,
            Matrix4x4 modelMatrix, float z
        );
    };
} // namespace PJ
