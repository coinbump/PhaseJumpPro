#ifndef PJRENDERMODEL_H
#define PJRENDERMODEL_H

#include "VectorList.h"
#include "Color.h"
#include "RenderMaterial.h"
#include "Vector3.h"
#include "Matrix4x4.h"

/*
 RATING: 4 stars
 Simple type. Needs more features
 CODE REVIEW: 4/17/23
 */
namespace PJ {
    class RenderTexture;
    class SomeShaderProgram;

    /// Sent to the render engine which then interprets it, does any
    /// necessary batching or caching, and then performs the render
    struct RenderModel {
        /// Vertices to render (sent to vertex shader `a_position` attribute)
        VectorList<Vector3> vertices;
        VectorList<Color> colors;

        /// Used by IBO to determine vertex offsets
        VectorList<uint32_t> indices;

        /// Texture position for vertex (Range: 0-1.0)
        VectorList<Vector2> uvs;

        VectorList<RenderTexture> textures;
        VectorList<Color> uniformColors;

        SomeShaderProgram& shaderProgram;

        Matrix4x4 matrix;

        RenderModel(SomeShaderProgram& shaderProgram) : shaderProgram(shaderProgram) {
        }
    };
}

#endif
