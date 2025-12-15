#include "FramePlayable.h"

using namespace PJ;
using namespace std;

FramePlayable::FramePlayable(int frameCount, float frameRate, AnimationCycleType cycleType) :
    frameRate(frameRate) {
    GUARD(frameRate > 0)

    auto duration = frameCount * (1.0f / frameRate);
    TimeTrack<int>::Config const& config{ .id = "frame.playable",
                                          .duration = duration,
                                          .cycleType = AnimationCycleType::Loop,
                                          .keyedTimeType = KeyedTimeType::Discrete };
    track = NEW<TimeTrack<int>>(config);
    controller = track.get();
    SetCycleType(cycleType);

    auto frameTime = 1.0f / frameRate;
    for (size_t i = 0; i < frameCount; i++) {
        track->AddAt(i * frameTime)->SetValue((int)i);
    }

    track->SetOnPlayTimeChangeFunc([this](auto& playable) { OnPlayTimeChange(); });
}

FramePlayable::FramePlayable(UP<TimeTrack<int>>& _track) {
    track = std::move(_track);
    controller = track.get();
    track->SetOnPlayTimeChangeFunc([this](auto& playable) { OnPlayTimeChange(); });
}

int FramePlayable::Frame() const {
    GUARDR(track, 0)

    auto keyframe = track->KeyframeAtOrBefore(track->PlayTime());
    GUARDR(keyframe, 0);

    return keyframe->Value();
}
