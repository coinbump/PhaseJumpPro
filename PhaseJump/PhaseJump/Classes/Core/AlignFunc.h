#pragma once

#include <functional>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/17/24
 */
namespace PJ {
    using AlignFunc = std::function<float(float layoutSize, float frameSize)>;

    namespace AlignFuncs {
        extern AlignFunc left;
        extern AlignFunc center;
        extern AlignFunc right;
    } // namespace AlignFuncs
} // namespace PJ
