#include "CycleHueEffect.h"
#include "SomeRenderer.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

void CycleHueEffect::Awake() {
    Base::Awake();

    UpdateEffect();
}

FinishType CycleHueEffect::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);

    GUARDR(IsEnabled(), FinishType::Continue);

    auto newHue = hue;
    newHue += time.delta / cycleTime;
    newHue = fmod(newHue, 1.0f);

    hue = newHue;
    UpdateEffect();

    return FinishType::Continue;
}

void CycleHueEffect::UpdateEffect() {
    GUARD(IsEnabled());

    auto target = Target();
    GUARD(target)

    auto renderer = target->GetComponent<SomeMaterialRenderer>();
    GUARD(renderer)

    ModelColor hsv;
    hsv.value = HSVColor(hue, saturation, value, 1.0f);
    ModelColor rgb = hsv.ToRGB();

    renderer->SetColor((Color)rgb);
}
