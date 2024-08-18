#include "EaseFunc.h"

using namespace std;
using namespace PJ;

const EaseFunc EaseFuncs::linear = [](float value) { return value; };
const EaseFunc EaseFuncs::inSquared = [](float value) { return powf(value, 2); };
const EaseFunc EaseFuncs::inCubed = [](float value) { return powf(value, 3); };
const EaseFunc EaseFuncs::outSquared = [](float value) { return 1.0f - powf(1.0f - value, 2); };
const EaseFunc EaseFuncs::outCubed = [](float value) { return 1.0f - powf(1.0f - value, 3); };

const EaseFunc EaseFuncs::outElastic = [](float value) {
    float x = value;
    float c4 = (2.0f * (float)M_PI) / 3.0f;
    return x == 0 ? 0 : x == 1 ? 1 : powf(2, -15 * x) * sin((x * 10.0f - 0.75f) * c4) + 1.0f;
};

const EaseFunc EaseFuncs::inBack = [](float value) {
    float x = value;
    float c1 = 1.70158f;
    float c3 = c1 + 1.0f;
    return c3 * x * x * x - c1 * x * x;
};

const EaseFunc EaseFuncs::outBack = [](float value) {
    float x = value;
    float c1 = 1.70158f;
    float c3 = c1 + 1.0f;
    return 1.0f + c3 * powf(x - 1, 3) + c1 * powf(x - 1, 2);
};

const EaseFunc EaseFuncs::inOutBack = [](float value) {
    float x = value;
    float c1 = 1.70158f;
    float c2 = c1 * 1.525f;
    return x < 0.5f ? (powf(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2.0f
                    : (powf(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2.0f;
};
