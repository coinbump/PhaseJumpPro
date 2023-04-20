#ifndef PJGLRENDERSTATE_H
#define PJGLRENDERSTATE_H

#include "GLHeaders.h"
#include "_Set.h"
#include "_String.h"
#include "_Map.h"
#include "GLBlendMode.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/14/23
 */
namespace PJ {
    /// Stores OpenGL render state for render debugging and deferred batch renders (future feature)
    class GLRenderState
    {
    public:
        struct Viewport {
            GLint x;
            GLint y;
            GLsizei width;
            GLsizei height;

            Viewport(GLint x, GLint y, GLsizei width, GLsizei height) : x(x), y(y), width(width), height(height) {
            }
        };
    protected:

    public:
        /*
         A frame buffer is the render destination for OpenGL calls
         */
        GLuint boundFrameBuffer = 0;
        GLuint boundVAO = -1;
        GLuint boundVBO = -1;
        GLuint boundIBO = -1;

        Set<String> enabledFeatures;
        GLBlendMode blendMode = GLBlendMode::standard;

        /*
         A vertex shader accepts attributes as inputs
         Each attribute has a unique location index
         To use these attributes we must enable them explicitly before rendering with the sahder
         */
        Set<GLuint> activeAttributeLocations;

        Viewport viewport{-1, -1, -1, -1};

        float lineWidth = 1;
    };
}

#endif
