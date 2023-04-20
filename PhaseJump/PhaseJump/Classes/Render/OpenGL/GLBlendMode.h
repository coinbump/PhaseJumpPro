#ifndef PJGLBLENDMODE_H
#define PJGLBLENDMODE_H

#include "GLHeaders.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/19/23
 */
namespace PJ {
    struct GLBlendMode {
        GLenum sFactor;
        GLenum dFactor;

        GLBlendMode(GLenum sFactor, GLenum dFactor) : sFactor(sFactor), dFactor(dFactor) {
        }

        static GLBlendMode const standard;
    };
}

#endif
