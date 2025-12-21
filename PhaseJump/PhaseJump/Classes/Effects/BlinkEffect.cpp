#include "BlinkEffect.h"
#include "Renderer.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

BlinkEffect::BlinkEffect(
    AnimationCycleType cycleType, VectorList<float> offKeys, VectorList<float> onKeys
) :
    timeTrack({ .duration = 0, .cycleType = cycleType, .keyedTimeType = KeyedTimeType::Discrete }) {
    float duration{};

    for (auto& time : offKeys) {
        duration = std::max(duration, time);
        timeTrack.AddAt(time)->value = false;
    }
    for (auto& time : onKeys) {
        duration = std::max(duration, time);
        timeTrack.AddAt(time)->value = true;
    }

    timeTrack.SetDuration(duration);

    timeTrack.setBindingFunc = [this](auto& value) {
        GUARD(owner)
        owner->Modify<Renderer>([=](auto& renderer) { renderer.Enable(value); });
    };

    attachmentCore.onEnabledChangeFunc = [](auto& core) {
        auto& effect = *(static_cast<This*>(&core.owner));

        if (core.IsEnabled()) {
            effect.timeTrack.Reset();
        } else {
            // Turn renderers back on when effect is off
            effect.owner->Modify<Renderer>([](auto& renderer) { renderer.Enable(true); });
        }
    };

    GetUpdatables().Add([this](TimeSlice time) { return timeTrack.OnUpdate(time); });
}
