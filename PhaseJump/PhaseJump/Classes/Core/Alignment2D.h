#pragma once

#include "HorizontalAlignment.h"
#include "VerticalAlignment.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// Combines horizontal and vertical alignment
    struct Alignment2D {
        AlignFunc xAlignFunc;
        AlignFunc yAlignFunc;

        static const Alignment2D center;
    };
} // namespace PJ
