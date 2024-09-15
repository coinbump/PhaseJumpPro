#include "AnimateHueEffect.h"
#include "SomeRenderer.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

void AnimateHueEffect::Awake() {
    Base::Awake();

    GUARD(owner);
    renderer = owner->GetComponent<SomeRenderer>();
}

void AnimateHueEffect::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);

    GUARD(IsOn());

    auto newHue = hue;
    newHue += time.delta / cycleTime;
    newHue = fmod(newHue, 1.0f);

    hue = newHue;
    UpdateEffectProperties();
}

void AnimateHueEffect::UpdateEffectProperties() {
    GUARD(IsOn());

    ModelColor hsv;
    hsv.value = HSVColor(hue, saturation, value, 1.0f);
    ModelColor rgb = hsv.ToRGB();

    auto lock = LOCK(renderer);
    GUARD(lock)
    lock->SetColor((Color)rgb);
}
