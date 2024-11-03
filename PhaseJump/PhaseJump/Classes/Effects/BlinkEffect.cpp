#include "BlinkEffect.h"
#include "SomeRenderer.h"
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
        owner->Modify<SomeRenderer>([=](auto& renderer) { renderer.Enable(value); });
    };

    onEnabledChangeFunc = [](auto& handler) {
        auto& effect = *(static_cast<This*>(&handler));

        if (handler.IsEnabled()) {
            effect.timeTrack.Reset();
        } else {
            // Turn renderers back on when effect is off
            effect.owner->Modify<SomeRenderer>([](auto& renderer) { renderer.Enable(true); });
        }
    };

    updatables.Add([this](TimeSlice time) {
        timeTrack.OnUpdate(time);
        return timeTrack.GetFinishType();
    });
}
