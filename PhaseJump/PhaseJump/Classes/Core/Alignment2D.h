#pragma once

#include "AlignFunc.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// Combines horizontal and vertical alignment
    struct Alignment2D {
        AlignFunc xAlignFunc = AlignFuncs::center;
        AlignFunc yAlignFunc = AlignFuncs::center;

        static const Alignment2D topLeading;
        static const Alignment2D top;
        static const Alignment2D topTrailing;
        static const Alignment2D leading;
        static const Alignment2D center;
        static const Alignment2D trailing;
        static const Alignment2D bottomLeading;
        static const Alignment2D bottom;
        static const Alignment2D bottomTrailing;
    };
} // namespace PJ
