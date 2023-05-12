#include "AnimateHueEffect.h"

using namespace std;
using namespace PJ;

void AnimateHueEffect::Awake() {
    Base::Awake();

    rendererTool = MAKE<RendererTool>(*LOCK(owner));
}

void AnimateHueEffect::OnUpdate(TimeSlice time)
{
    Base::OnUpdate(time);

    if (!IsOn()) { return; }

    auto newHue = hue;
    newHue += time.delta / cycleTime;
    newHue = fmod(newHue, 1.0f);

    hue = newHue;
    UpdateEffectProperties();
}

void AnimateHueEffect::UpdateEffectProperties()
{
    Base::UpdateEffectProperties();

    if (!IsOn()) { return; }
    if (nullptr == rendererTool) { return; }

    ModelColor hsv;
    hsv.value = HSVColor(hue, saturation, value, 1.0f);
    ModelColor rgb = hsv.ToRGB();

    rendererTool->SetColor(rgb);
}
