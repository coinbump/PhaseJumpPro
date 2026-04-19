#include "ExampleEaseScene.h"

using namespace std;
using namespace PJ;
using namespace Example;

EaseScene::EaseScene() {
    easeFuncs = {
        { "linear", EaseFuncs::linear },

        { "inSine", EaseFuncs::inSine },
        { "outSine", EaseFuncs::outSine },
        { "inOutSine", EaseFuncs::inOutSine },

        { "inSquared", EaseFuncs::inSquared },
        { "outSquared", EaseFuncs::outSquared },
        { "inOutSquared", EaseFuncs::inOutSquared },

        { "inCubed", EaseFuncs::inCubed },
        { "outCubed", EaseFuncs::outCubed },
        { "inOutCubed", EaseFuncs::inOutCubed },

        { "inQuart", EaseFuncs::inQuart },
        { "outQuart", EaseFuncs::outQuart },
        { "inOutQuart", EaseFuncs::inOutQuart },

        { "inQuint", EaseFuncs::inQuint },
        { "outQuint", EaseFuncs::outQuint },
        { "inOutQuint", EaseFuncs::inOutQuint },

        { "inExpo", EaseFuncs::inExpo },
        { "outExpo", EaseFuncs::outExpo },
        { "inOutExpo", EaseFuncs::inOutExpo },

        { "inCirc", EaseFuncs::inCirc },
        { "outCirc", EaseFuncs::outCirc },
        { "inOutCirc", EaseFuncs::inOutCirc },

        { "inBack", EaseFuncs::inBack },
        { "outBack", EaseFuncs::outBack },
        { "inOutBack", EaseFuncs::inOutBack },

        { "inElastic", EaseFuncs::inElastic },
        { "outElastic", EaseFuncs::outElastic },
        { "inOutElastic", EaseFuncs::inOutElastic },

        { "inBounce", EaseFuncs::inBounce },
        { "outBounce", EaseFuncs::outBounce },
        { "inOutBounce", EaseFuncs::inOutBounce },
    };

    PlanUIFunc planUIFunc = [this](auto args) {
        auto& planner = args.planner;

        planner.ListSelect([this]() {
            VectorList<String> options =
                Map<String>(easeFuncs, [](auto& entry) { return entry.name; });

            Binding<int> binding({ .getFunc =
                                       [this]() {
                                           auto current = selectedEaseFunc.Value();
                                           auto result =
                                               IndexOfIf(easeFuncs, [current](auto& entry) {
                                                   return entry.func == current;
                                               });
                                           return result ? (int)*result : -1;
                                       },
                                   .setFunc =
                                       [this](auto& value) {
                                           GUARD(value >= 0 && value < (int)easeFuncs.size())
                                           selectedEaseFunc = easeFuncs[value].func;
                                       } });

            return UIPlanner::ListSelectConfig{ .label = "Ease",
                                                .options = options,
                                                .binding = binding };
        });
    };

    Override(planUIFuncs[UIContextId::Editor], planUIFunc);
}

void EaseScene::LoadInto(WorldNode& root) {
    QB(root)
        .Named("Example: Ease")
        .OrthoStandard()
        .With<ImRenderer>(ImRenderer::Config{ .worldSize = root.World()->Window()->PixelSize(),
                                              .areShapesOpaque = true })
        .SizeWithWindow()
        .ModifyLatest<ImRenderer>([this](auto& renderer) {
            renderNode = renderer.owner;

            renderer.AddSignalHandler({ .id = SignalId::RenderPrepare,
                                        .func = [this](auto& renderer, auto& signal) {
                                            ImRenderer& im = *(static_cast<ImRenderer*>(&renderer));

                                            Vector2 imSize = im.WorldSize();
                                            GUARD(imSize.x > 0 && imSize.y > 0)

                                            im.FillRect({ .size = imSize }, Color::black);

                                            float squareSize = std::min(imSize.x, imSize.y) * 0.7f;
                                            Vector2 squareOrigin{ (imSize.x - squareSize) / 2.0f,
                                                                  (imSize.y - squareSize) / 2.0f };
                                            Rect squareFrame{ squareOrigin,
                                                              { squareSize, squareSize } };

                                            im.SetStrokeColor(Color::white).SetStrokeWidth(3);
                                            im.FrameRect(squareFrame);

                                            // Sample the ease func at many steps and draw as a
                                            // PolyLine.
                                            auto easeFunc = selectedEaseFunc.Value();
                                            GUARD(easeFunc)

                                            constexpr int kSampleCount = 128;
                                            Polygon poly;
                                            for (int i = 0; i <= kSampleCount; i++) {
                                                float t = (float)i / (float)kSampleCount;
                                                float eased = easeFunc(t);
                                                float x = squareOrigin.x + t * squareSize;
                                                float y = squareOrigin.y + eased * squareSize;
                                                poly.Add(Vector2{ x, y });
                                            }

                                            im.SetStrokeWidth(5).PolyLine(poly);

                                            // Red circle follows the plotted curve
                                            float easedAtProgress = easeFunc(progress);
                                            Vector2 circleOrigin{
                                                squareOrigin.x + progress * squareSize,
                                                squareOrigin.y + easedAtProgress * squareSize
                                            };
                                            im.FillCircle(circleOrigin, 30.0f, Color::red);
                                        } });
        });

    selectedEaseFunc.SetOnValueChangeFunc(SetOnValueChangeFuncType::None, [this](auto value) {
        OnEaseFuncChanged();
    });
}

void EaseScene::OnEaseFuncChanged() {
    GUARD(renderNode)

    progress = 0;

    renderNode->updatables.RemoveAll();

    auto timer = MAKE<Timer>(Timer::Config{
        .duration = timerDuration, .runType = RunType::Once, .onFinishFunc = {} });

    renderNode->updatables.AddContinue([this, timer](TimeSlice time) {
        timer->OnUpdate(time);
        progress = timer->Progress();
    });
}
