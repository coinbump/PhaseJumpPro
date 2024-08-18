#ifndef PJKEYFRAME_H
#define PJKEYFRAME_H

#include "SomeKeyframe.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/5/24
 */
namespace PJ {
    template <class T>
    class ValueKeyframe : public SomeKeyframe {
    public:
        T value;
    };
} // namespace PJ

#endif
