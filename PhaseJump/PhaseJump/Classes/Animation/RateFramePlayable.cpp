#include "RateFramePlayable.h"

using namespace PJ;
using namespace std;

int RateFramePlayable::Frame() const {
    auto count = frameCount;
    GUARDR(count > 0, 0)

    float frameTime = FrameTime();
    int frame = timer.PlayTime() / frameTime;
    frame = clamp(frame, 0, int(count) - 1);

    return frame;
}
