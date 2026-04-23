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

float FramePlayable::FrameRate() const {
    GUARDR(track, frameRate)

    auto const& keyframes = track->Keyframes();
    auto const n = keyframes.size();
    GUARDR(n >= 2, frameRate)

    // Average interval between consecutive keyframes across the span from first to last
    float const span = keyframes[n - 1]->time - keyframes[0]->time;
    GUARDR(span > 0, frameRate)

    float const averageInterval = span / static_cast<float>(n - 1);
    return 1.0f / averageInterval;
}

void FramePlayable::SetFrameRate(float value) {
    GUARD(value > 0)
    GUARD(track)

    frameRate = value;

    float const frameTime = 1.0f / value;

    // Redistribute existing keyframes at uniform `frameTime` intervals
    auto const& keyframes = track->Keyframes();
    for (size_t i = 0; i < keyframes.size(); i++) {
        keyframes[i]->time = static_cast<float>(i) * frameTime;
    }

    SetDuration(static_cast<float>(keyframes.size()) * frameTime);
}
