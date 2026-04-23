#pragma once

#include "GLHeaders.h"
#include "GLRenderEngine.h"
#include "Macros.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/21/26
 */
namespace PJ {
    /// RAII guard that captures the currently-bound frame buffer and render buffer on
    /// construction and restores them on destruction. Use this around any ad-hoc framebuffer
    /// bind that happens outside of the main render pass
    class GLBufferBindingScope {
    public:
        DELETE_COPY(GLBufferBindingScope)
        DELETE_MOVE(GLBufferBindingScope)

        explicit GLBufferBindingScope(GLRenderEngine& renderEngine) :
            renderEngine(renderEngine) {
            glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFrameBuffer);
            glGetIntegerv(GL_RENDERBUFFER_BINDING, &prevRenderBuffer);
        }

        ~GLBufferBindingScope() {
            renderEngine.BindFrameBuffer((GLuint)prevFrameBuffer);
            renderEngine.BindRenderBuffer(GL_RENDERBUFFER, (GLuint)prevRenderBuffer);
        }

    private:
        GLRenderEngine& renderEngine;
        GLint prevFrameBuffer{};
        GLint prevRenderBuffer{};
    };
} // namespace PJ
