#include "SomeFrameAnimator.h"

using namespace PJ;
using namespace std;

int RateFrameAnimator::FrameForUpdate(TimeSlice time, int frameCount) {
    auto count = frameCount;
    if (count < 1) {
        return 0;
    }
    if (rate < 0) {
        return frame;
    }

    float frameTime = 1.0f / rate;
    float duration = count * frameTime;

    position += time.delta;
    position = fmodf(position, duration);
    frame = position/frameTime;

    frame = clamp(frame, 0, int(count) - 1);

    return frame;
}
