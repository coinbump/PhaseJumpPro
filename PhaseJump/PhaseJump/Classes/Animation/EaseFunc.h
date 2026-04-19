#pragma once

/*
 RATING: 5+ stars
 Simple type
 CODE REVIEW: 4/17/26
 */
namespace PJ {
    /// Transforms a normalized progress value (0-1.0) to a new normalized value.
    ///
    /// Most easings stay within [0, 1], but the Back and Elastic families intentionally
    /// overshoot the endpoints.
    ///
    /// Formulas follow https://easings.net
    using EaseFunc = float (*)(float);

    namespace EaseFuncs {
        extern const EaseFunc linear;

        extern const EaseFunc inSine;
        extern const EaseFunc outSine;
        extern const EaseFunc inOutSine;

        extern const EaseFunc inSquared;
        extern const EaseFunc outSquared;
        extern const EaseFunc inOutSquared;

        extern const EaseFunc inCubed;
        extern const EaseFunc outCubed;
        extern const EaseFunc inOutCubed;

        extern const EaseFunc inQuart;
        extern const EaseFunc outQuart;
        extern const EaseFunc inOutQuart;

        extern const EaseFunc inQuint;
        extern const EaseFunc outQuint;
        extern const EaseFunc inOutQuint;

        extern const EaseFunc inExpo;
        extern const EaseFunc outExpo;
        extern const EaseFunc inOutExpo;

        extern const EaseFunc inCirc;
        extern const EaseFunc outCirc;
        extern const EaseFunc inOutCirc;

        extern const EaseFunc inBack;
        extern const EaseFunc outBack;
        extern const EaseFunc inOutBack;

        extern const EaseFunc inElastic;
        extern const EaseFunc outElastic;
        extern const EaseFunc inOutElastic;

        extern const EaseFunc inBounce;
        extern const EaseFunc outBounce;
        extern const EaseFunc inOutBounce;
    } // namespace EaseFuncs
} // namespace PJ
