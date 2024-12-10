#pragma once

#include "SomeAligner.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/14/23
 */
namespace PJ {
    /// Horizontal alignment in a horizontal flow
    namespace HorizontalAlignment {
        extern const AlignFunc leading;
        extern const AlignFunc center;
        extern const AlignFunc trailing;
    }; // namespace HorizontalAlignment
} // namespace PJ
