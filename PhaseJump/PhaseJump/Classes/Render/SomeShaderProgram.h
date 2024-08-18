#ifndef PJSOMESHADERPROGRAM_H
#define PJSOMESHADERPROGRAM_H

#include "_String.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/8/24
 */
namespace PJ {
    class SomeShaderProgram {
    public:
        String id;

        virtual ~SomeShaderProgram() {}
    };
} // namespace PJ

#endif
