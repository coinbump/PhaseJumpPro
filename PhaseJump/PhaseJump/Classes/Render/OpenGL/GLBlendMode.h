#pragma once

#include "GLHeaders.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/18/24
 */
namespace PJ {
    struct GLBlendMode {
        GLenum sFactor;
        GLenum dFactor;

        GLBlendMode(GLenum sFactor, GLenum dFactor) :
            sFactor(sFactor),
            dFactor(dFactor) {}

        static GLBlendMode const standard;
    };
} // namespace PJ
