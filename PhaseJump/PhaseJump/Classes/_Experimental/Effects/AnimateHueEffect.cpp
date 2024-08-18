#include "AnimateHueEffect.h"

using namespace std;
using namespace PJ;

void AnimateHueEffect::Awake() {
    Base::Awake();

    GUARD(owner)
    rendererTool = MAKE<RendererTool>(*owner);
}

void AnimateHueEffect::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);
    //
    //    GUARD(switchHandler && switchHandler->IsOn());
    //
    //    auto newHue = hue;
    //    newHue += time.delta / cycleTime;
    //    newHue = fmod(newHue, 1.0f);
    //
    //    hue = newHue;
    //    UpdateEffectProperties();
}

void AnimateHueEffect::UpdateEffectProperties() {
    //    GUARD(switchHandler && switchHandler->IsOn());
    //    GUARD(rendererTool)
    //
    //    ModelColor hsv;
    //    hsv.value = HSVColor(hue, saturation, value, 1.0f);
    //    ModelColor rgb = hsv.ToRGB();
    //
    //    rendererTool->SetColor(rgb);
}
