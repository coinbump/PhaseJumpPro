#pragma once

#include "Base.h"
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
    class SomeShader : public Base {};

    /// Manages an OpenGLShader
    class GLShader : public SomeShader {
    public:
        GLuint glId{};
        GLenum glType{};
        bool isCompiled{};

        GLShader();
        virtual ~GLShader();

        void Destroy();
    };

    // MARK: - FragmentGLShader

    /// Fragment shaders render pixels (fragments)
    class FragmentGLShader : public GLShader {
    public:
        FragmentGLShader();
    };

    // MARK: - VertexGLShader

    /// Vertex shaders process vertexes
    class VertexGLShader : public GLShader {
    public:
        VertexGLShader();
    };
} // namespace PJ
