#pragma once

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/13/24
 */
namespace PJ {
    using EaseFunc = std::function<float(float)>;

    namespace EaseFuncs {
        extern const EaseFunc linear;
        extern const EaseFunc inSquared;
        extern const EaseFunc inCubed;
        extern const EaseFunc outSquared;
        extern const EaseFunc outCubed;
        extern const EaseFunc outElastic;
        extern const EaseFunc inBack;
        extern const EaseFunc outBack;
        extern const EaseFunc inOutBack;
    } // namespace EaseFuncs
} // namespace PJ
