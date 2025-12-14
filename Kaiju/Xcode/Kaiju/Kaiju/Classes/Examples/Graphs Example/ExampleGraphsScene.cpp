#include "ExampleGraphsScene.h"

using namespace std;
using namespace PJ;
using namespace Example;

Graphs::Scene::Scene() {
    sceneModel = { .maxAxisValue = 1000,
                   .bars = {
                       { .name = "January" },
                       { .name = "February" },
                       { .name = "March" },
                       { .name = "April" },
                       { .name = "May" },
                       { .name = "June" },
                       { .name = "July" },
                       { .name = "August" },
                       { .name = "September" },
                       { .name = "October" },
                       { .name = "November" },
                       { .name = "December" },
                   } };
}

void Graphs::Scene::LoadInto(WorldNode& root) {
    QB(root)
        .Named("Example: Graphs")
        .OrthoStandard(QuickBuilder::OrthoStandardConfig{ .clearColor = Color::black })
        .And("Graphs")
        .RootView(
            worldSize,
            [this](auto& vb) {
                vb.VStack({ .spacing = 20, .buildViewFunc = [this](auto& vb) {
                               vb.Immediate({ .renderFunc = [this](auto& view, auto& renderer) {
                                   /// Optimize: draw opaque shapes
                                   renderer.areShapesOpaque = true;

                                   auto labelColumnWidth = renderer.WorldSize().x * 0.3f;
                                   auto barColumnWidth = renderer.WorldSize().x - labelColumnWidth;
                                   int barCount = (int)sceneModel.bars.size();
                                   auto spacing = 10.0f;
                                   auto barHeight =
                                       (renderer.WorldSize().y - (barCount - 1) * spacing) /
                                       barCount;

                                   auto y = renderer.WorldSize().y * vecUp + barHeight * vecDown;
                                   for (auto& bar : sceneModel.bars) {
                                       auto ratio = bar.value / sceneModel.maxAxisValue;

                                       renderer.Text(bar.name, Vector2{ 0, y }, 32, Color::white);

                                       auto x = labelColumnWidth;
                                       Rect barFrame({ .origin = { x, y },
                                                       .size = { barColumnWidth * ratio,
                                                                 barHeight } });

                                       renderer.FillRect(barFrame, Color::blue);

                                       y += (barHeight + spacing) * vecDown;
                                   }
                               } });

                               vb.Button({ .label = "Randomize",
                                           .onPressFunc = [this](auto& button) {
                                               RandomizeValues(true);
                                           } });
                           } });
            }
        )
        .SizeWithWindow({ 0.5f, 0.75f });

    RandomizeValues(true);
}

void Graphs::Scene::RandomizeValues(bool isAnimated) {
    StandardRandom random;

    GetUpdatables().RemoveAll();

    for (auto& bar : sceneModel.bars) {
        auto oldValue = bar.value;
        float newValue = random.Value() * (sceneModel.maxAxisValue - sceneModel.minAxisValue) +
                         sceneModel.minAxisValue;

        if (isAnimated) {
            InterpolateFunc<float> interpolateFunc = InterpolateFuncs::Ease(
                InterpolateFuncs::Make(oldValue, newValue), EaseFuncs::outElastic
            );
            auto animator = NEW<Animator<float>>(Animator<float>::Config{
                .interpolateFunc = interpolateFunc, .duration = 1, .binding = [&](auto value) {
                    bar.value = value;
                } });

            GetUpdatables().Add(std::move(animator));
        } else {
            bar.value = newValue;
        }
    }
}
