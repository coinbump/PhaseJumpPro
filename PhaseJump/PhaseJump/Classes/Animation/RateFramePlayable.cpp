#include "RateFramePlayable.h"

using namespace PJ;
using namespace std;

int RateFramePlayable::Frame() const {
    GUARDR(playTimeController, 0)

    auto count = frameCount;
    GUARDR(count > 0, 0)

    float frameTime = 1.0f / rate;
    int frame = playTimeController->Position() / frameTime;
    frame = clamp(frame, 0, int(count) - 1);

    return frame;
}
