#include "TextView.h"
#include "TextMeasurer.h"
#include "TextRenderer.h"
#include "World.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

TextRenderer* TextView::Renderer() {
    GUARDR(renderer.expired(), renderer.lock().get())
    GUARDR(owner, {})

    auto world = owner->World();
    GUARDR(world, {})

    auto font = FindFont(world->resources, config.fontSpec);
    GUARDR_LOG(font, {}, "ERROR: Missing font");

    owner->AddComponent<TextRenderer>(TextRenderer::Config{
        .font = font, .text = config.text, .worldSize = frame.size });
    renderer = owner->TypeComponentPtr<TextRenderer>();
    renderer.lock()->SetColor(config.color);
    return renderer.lock().get();
}

TextView::TextView() {
    IdealSizeFunc idealSizeFunc = [this](auto& view, auto proposal) {
        auto _renderer = Renderer();
        GUARDR(_renderer, ViewSizeProposal{})

        auto font = _renderer->GetFont();
        GUARDR(font, ViewSizeProposal{})

        Vector2 textSize(
            proposal.width ? *proposal.width : FloatMath::maxValue,
            proposal.height ? *proposal.height : FloatMath::maxValue
        );

        TextMeasurer tm(*font);
        auto metrics = tm.Measure(_renderer->Text(), textSize, _renderer->GetLineClip());

        auto calculatedSize = metrics.CalculateSize(metrics.lines);
        return ViewSizeProposal{ .width = calculatedSize.x, .height = calculatedSize.y };
    };

    SetIsIdealSize(false, true);
    SetIdealSizeFunc(idealSizeFunc);
}

TextView::TextView(Config const& config) :
    TextView() {
    this->config = config;
}

void TextView::SetText(String value) {
    auto renderer = Renderer();
    GUARD(renderer);
    GUARD(value != renderer->PlainText())

    renderer->SetText(value);
    SetNeedsLayout();
}

void TextView::Configure(Config value) {
    config = value;

    if (!renderer.expired()) {
        owner->Remove(*renderer.lock());
        renderer.reset();
    }

    SetNeedsLayout();
}
