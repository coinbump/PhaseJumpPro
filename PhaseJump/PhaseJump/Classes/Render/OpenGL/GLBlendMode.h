#pragma once

#include "GLHeaders.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 11/16/24
 */
namespace PJ {
    /// Stores parameters for OpenGL blend mode
    struct GLBlendMode {
        GLenum sourceFactor;
        GLenum destFactor;

        GLBlendMode(GLenum sourceFactor, GLenum destFactor) :
            sourceFactor(sourceFactor),
            destFactor(destFactor) {}

        static GLBlendMode const standard;
    };
} // namespace PJ
