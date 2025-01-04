#pragma once

#include "GLBlendMode.h"
#include "GLHeaders.h"
#include "OrderedMap.h"
#include "StringUtils.h"
#include "UnorderedSet.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/8/24
 */
namespace PJ {
    /// Stores OpenGL render state for render debugging and deferred batch
    /// renders (future feature)
    class GLRenderState {
    public:
        struct Viewport {
            GLint x = 0;
            GLint y = 0;
            GLsizei width = 0;
            GLsizei height = 0;

            Viewport(GLint x, GLint y, GLsizei width, GLsizei height) :
                x(x),
                y(y),
                width(width),
                height(height) {}
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

        UnorderedSet<String> enabledFeatures;
        GLBlendMode blendMode = GLBlendMode::standard;

        UnorderedSet<uint32_t> clearedContexts;

        void RenderPassStart() {
            clearedContexts.clear();
        }

        bool IsContextCleared(uint32_t id) {
            return clearedContexts.contains(id);
        }

        void SetIsContextCleared(uint32_t id, bool value) {
            AddOrRemove(clearedContexts, id, value);
        }

        /*
         A vertex shader accepts attributes as inputs
         Each attribute has a unique location index
         To use these attributes we must enable them explicitly before rendering
         with the sahder
         */
        UnorderedSet<GLuint> activeAttributeLocations;

        Viewport viewport{ -1, -1, -1, -1 };

        float lineWidth = 1;

        bool IsFeatureEnabled(String featureId) {
            return enabledFeatures.contains(featureId);
        }
    };
} // namespace PJ
