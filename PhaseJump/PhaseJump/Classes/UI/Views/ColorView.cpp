#include "ColorView.h"
#include "ColorRenderer.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

ColorView::ColorView(Color color) :
    color(color) {}

ColorRenderer* ColorView::Renderer() {
    auto result =
        &owner->AddComponentIfNeeded<ColorRenderer>(ColorRenderer::Config{ .color = color });

    // All views must use blend mode so they are Z-ordered correctly
    result->EnableBlend(true);

    return result;
}

void ColorView::Awake() {
    Base::Awake();

    Renderer();
}

void ColorView::SetColor(Color value) {
    auto renderer = Renderer();
    GUARD(renderer)

    color = value;
    renderer->SetColor(value);
}
