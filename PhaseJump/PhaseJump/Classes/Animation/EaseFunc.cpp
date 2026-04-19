#include "EaseFunc.h"
#include <cmath>
#include <numbers>

using namespace std;
using namespace PJ;

// All formulas follow https://easings.net

namespace {
    constexpr float kPi = std::numbers::pi_v<float>;

    // Shared helper for the Bounce family
    float BounceOut(float value) {
        constexpr float n1 = 7.5625f;
        constexpr float d1 = 2.75f;

        if (value < 1.0f / d1) {
            return n1 * value * value;
        } else if (value < 2.0f / d1) {
            value -= 1.5f / d1;
            return n1 * value * value + 0.75f;
        } else if (value < 2.5f / d1) {
            value -= 2.25f / d1;
            return n1 * value * value + 0.9375f;
        }
        value -= 2.625f / d1;
        return n1 * value * value + 0.984375f;
    }
} // namespace

const EaseFunc EaseFuncs::linear = [](float value) { return value; };

// https://easings.net/#easeInSine
const EaseFunc EaseFuncs::inSine = [](float value) { return 1.0f - cosf((value * kPi) / 2.0f); };

// https://easings.net/#easeOutSine
const EaseFunc EaseFuncs::outSine = [](float value) { return sinf((value * kPi) / 2.0f); };

// https://easings.net/#easeInOutSine
const EaseFunc EaseFuncs::inOutSine = [](float value) {
    return -(cosf(kPi * value) - 1.0f) / 2.0f;
};

// https://easings.net/#easeInQuad
const EaseFunc EaseFuncs::inSquared = [](float value) { return value * value; };

// https://easings.net/#easeOutQuad
const EaseFunc EaseFuncs::outSquared = [](float value) {
    float inv = 1.0f - value;
    return 1.0f - inv * inv;
};

// https://easings.net/#easeInOutQuad
const EaseFunc EaseFuncs::inOutSquared = [](float value) {
    if (value < 0.5f) {
        return 2.0f * value * value;
    }
    float inv = -2.0f * value + 2.0f;
    return 1.0f - (inv * inv) / 2.0f;
};

// https://easings.net/#easeInCubic
const EaseFunc EaseFuncs::inCubed = [](float value) { return value * value * value; };

// https://easings.net/#easeOutCubic
const EaseFunc EaseFuncs::outCubed = [](float value) {
    float inv = 1.0f - value;
    return 1.0f - inv * inv * inv;
};

// https://easings.net/#easeInOutCubic
const EaseFunc EaseFuncs::inOutCubed = [](float value) {
    if (value < 0.5f) {
        return 4.0f * value * value * value;
    }
    float inv = -2.0f * value + 2.0f;
    return 1.0f - (inv * inv * inv) / 2.0f;
};

// https://easings.net/#easeInQuart
const EaseFunc EaseFuncs::inQuart = [](float value) { return value * value * value * value; };

// https://easings.net/#easeOutQuart
const EaseFunc EaseFuncs::outQuart = [](float value) {
    float inv = 1.0f - value;
    return 1.0f - inv * inv * inv * inv;
};

// https://easings.net/#easeInOutQuart
const EaseFunc EaseFuncs::inOutQuart = [](float value) {
    if (value < 0.5f) {
        return 8.0f * value * value * value * value;
    }
    float inv = -2.0f * value + 2.0f;
    return 1.0f - (inv * inv * inv * inv) / 2.0f;
};

// https://easings.net/#easeInQuint
const EaseFunc EaseFuncs::inQuint = [](float value) {
    return value * value * value * value * value;
};

// https://easings.net/#easeOutQuint
const EaseFunc EaseFuncs::outQuint = [](float value) {
    float inv = 1.0f - value;
    return 1.0f - inv * inv * inv * inv * inv;
};

// https://easings.net/#easeInOutQuint
const EaseFunc EaseFuncs::inOutQuint = [](float value) {
    if (value < 0.5f) {
        return 16.0f * value * value * value * value * value;
    }
    float inv = -2.0f * value + 2.0f;
    return 1.0f - (inv * inv * inv * inv * inv) / 2.0f;
};

// https://easings.net/#easeInExpo
const EaseFunc EaseFuncs::inExpo = [](float value) {
    return value == 0.0f ? 0.0f : powf(2.0f, 10.0f * value - 10.0f);
};

// https://easings.net/#easeOutExpo
const EaseFunc EaseFuncs::outExpo = [](float value) {
    return value == 1.0f ? 1.0f : 1.0f - powf(2.0f, -10.0f * value);
};

// https://easings.net/#easeInOutExpo
const EaseFunc EaseFuncs::inOutExpo = [](float value) {
    if (value == 0.0f) {
        return 0.0f;
    }
    if (value == 1.0f) {
        return 1.0f;
    }
    if (value < 0.5f) {
        return powf(2.0f, 20.0f * value - 10.0f) / 2.0f;
    }
    return (2.0f - powf(2.0f, -20.0f * value + 10.0f)) / 2.0f;
};

// https://easings.net/#easeInCirc
const EaseFunc EaseFuncs::inCirc = [](float value) { return 1.0f - sqrtf(1.0f - value * value); };

// https://easings.net/#easeOutCirc
const EaseFunc EaseFuncs::outCirc = [](float value) {
    float d = value - 1.0f;
    return sqrtf(1.0f - d * d);
};

// https://easings.net/#easeInOutCirc
const EaseFunc EaseFuncs::inOutCirc = [](float value) {
    if (value < 0.5f) {
        float d = 2.0f * value;
        return (1.0f - sqrtf(1.0f - d * d)) / 2.0f;
    }
    float d = -2.0f * value + 2.0f;
    return (sqrtf(1.0f - d * d) + 1.0f) / 2.0f;
};

// https://easings.net/#easeInBack
const EaseFunc EaseFuncs::inBack = [](float value) {
    constexpr float c1 = 1.70158f;
    constexpr float c3 = c1 + 1.0f;
    return c3 * value * value * value - c1 * value * value;
};

// https://easings.net/#easeOutBack
const EaseFunc EaseFuncs::outBack = [](float value) {
    constexpr float c1 = 1.70158f;
    constexpr float c3 = c1 + 1.0f;
    float d = value - 1.0f;
    return 1.0f + c3 * d * d * d + c1 * d * d;
};

// https://easings.net/#easeInOutBack
const EaseFunc EaseFuncs::inOutBack = [](float value) {
    constexpr float c1 = 1.70158f;
    constexpr float c2 = c1 * 1.525f;
    if (value < 0.5f) {
        float t = 2.0f * value;
        return (t * t * ((c2 + 1.0f) * 2.0f * value - c2)) / 2.0f;
    }
    float t = 2.0f * value - 2.0f;
    return (t * t * ((c2 + 1.0f) * (value * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
};

// https://easings.net/#easeInElastic
const EaseFunc EaseFuncs::inElastic = [](float value) {
    constexpr float c4 = (2.0f * kPi) / 3.0f;
    if (value == 0.0f) {
        return 0.0f;
    }
    if (value == 1.0f) {
        return 1.0f;
    }
    return -powf(2.0f, 10.0f * value - 10.0f) * sinf((value * 10.0f - 10.75f) * c4);
};

// https://easings.net/#easeOutElastic
const EaseFunc EaseFuncs::outElastic = [](float value) {
    constexpr float c4 = (2.0f * kPi) / 3.0f;
    if (value == 0.0f) {
        return 0.0f;
    }
    if (value == 1.0f) {
        return 1.0f;
    }
    return powf(2.0f, -10.0f * value) * sinf((value * 10.0f - 0.75f) * c4) + 1.0f;
};

// https://easings.net/#easeInOutElastic
const EaseFunc EaseFuncs::inOutElastic = [](float value) {
    constexpr float c5 = (2.0f * kPi) / 4.5f;
    if (value == 0.0f) {
        return 0.0f;
    }
    if (value == 1.0f) {
        return 1.0f;
    }
    if (value < 0.5f) {
        return -(powf(2.0f, 20.0f * value - 10.0f) * sinf((20.0f * value - 11.125f) * c5)) / 2.0f;
    }
    return (powf(2.0f, -20.0f * value + 10.0f) * sinf((20.0f * value - 11.125f) * c5)) / 2.0f +
           1.0f;
};

// https://easings.net/#easeInBounce
const EaseFunc EaseFuncs::inBounce = [](float value) { return 1.0f - BounceOut(1.0f - value); };

// https://easings.net/#easeOutBounce
const EaseFunc EaseFuncs::outBounce = [](float value) { return BounceOut(value); };

// https://easings.net/#easeInOutBounce
const EaseFunc EaseFuncs::inOutBounce = [](float value) {
    if (value < 0.5f) {
        return (1.0f - BounceOut(1.0f - 2.0f * value)) / 2.0f;
    }
    return (1.0f + BounceOut(2.0f * value - 1.0f)) / 2.0f;
};
