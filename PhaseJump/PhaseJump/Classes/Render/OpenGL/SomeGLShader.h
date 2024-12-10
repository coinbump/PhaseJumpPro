#pragma once

#include "FilePath.h"
#include "GLHeaders.h"
#include "StringUtils.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// Interfaces with the GPU to process vertices/fragments
    class SomeShader {};

    /// Manages an OpenGLShader
    class SomeGLShader : public SomeShader {
    public:
        GLuint glId{};
        GLenum glType{};
        bool isCompiled{};

        SomeGLShader();
        virtual ~SomeGLShader();

        void Destroy();
    };

    // MARK: - FragmentGLShader

    /// Fragment shaders render pixels (fragments)
    class FragmentGLShader : public SomeGLShader {
    public:
        FragmentGLShader();
    };

    // MARK: - VertexGLShader

    /// Vertex shaders process vertexes
    class VertexGLShader : public SomeGLShader {
    public:
        VertexGLShader();
    };
} // namespace PJ
