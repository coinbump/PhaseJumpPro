#ifndef PJSOMEGLSHADER_H
#define PJSOMEGLSHADER_H

#include "FilePath.h"
#include "GLHeaders.h"
#include "StringUtils.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 7/14/24
 */
namespace PJ {
    /// Interfaces with the GPU to process vertices/fragments
    class SomeShader {};

    /// Manages an OpenGLShader
    class SomeGLShader : public SomeShader {
    public:
        GLuint glId = 0;
        GLenum glType = 0;
        bool isCompiled = false;

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

#endif
