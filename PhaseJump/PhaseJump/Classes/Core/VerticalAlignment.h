#pragma once

#include "SomeAligner.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Utility
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// Vertical alignment in a horizontal flow
    namespace VerticalAlignment {
        extern const AlignFunc top;
        extern const AlignFunc center;
        extern const AlignFunc bottom;
    }; // namespace VerticalAlignment
} // namespace PJ
