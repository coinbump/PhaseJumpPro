#include "AnimateHueEffect.h"
#include "SomeRenderer.h"

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
    GUARD(renderer)

    ModelColor hsv;
    hsv.value = HSVColor(hue, saturation, value, 1.0f);
    ModelColor rgb = hsv.ToRGB();

    renderer->SetColor((Color)rgb);
}
