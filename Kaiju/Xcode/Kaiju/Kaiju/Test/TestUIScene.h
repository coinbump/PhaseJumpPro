#pragma once

#if DEVELOPMENT
#include <PhaseJump-Dev/PhaseJump-Dev.h>
#else
#include <PhaseJump/PhaseJump.h>
#endif

using namespace PJ;

class TestUIScene : public Scene {
public:
    SP<BezierPathLayout> bezierLayout;

    VectorList<Vector3> controlPoints{{-300, 0, 0}, {-150, 300, 0}, {300, 300, 0}, {300, 450, 0}};

    TestUIScene() {
    }

    void LoadInto(WorldNode& root) {
        auto world = root.World();
        auto font = world->FindFontWithResourceId("ArialBlack-32");
        GUARD(font)
        auto buttonTexture = DCAST<GLTexture>(root.World()->FindTexture("example-button-normal"));
        GUARD(buttonTexture)

        QB(root)
            .Named("TestUIScene")
            .With<OrthoCamera>()
            .With<SimpleRaycaster2D>()
            .And("Drag Gesture")
            .Circle(20)
            .CircleCollider(20)
            .With<SomeDragGestureHandler2D>()
            .And("label")
            .With<TextRenderer>(font, "Delta:", Vector2(400, 400))
            .SetLocalPosition({0, -30, 0})
            .ModifyLatest<SomeDragGestureHandler2D>([](SomeDragGestureHandler2D& c) {
                auto childComponents = c.owner->GetComponentsInChildren<TextRenderer>();
                GUARD(!IsEmpty(childComponents))
                auto textRenderer = childComponents[0];

                c.onDragGestureUpdateFunc = [=](SomeDragGestureHandler2D::Update update) {
                    switch (update.type) {
                        case SomeDragGestureHandler2D::Update::Type::Start:
                            std::cout << "Drag start" << std::endl;
                            break;
                        case SomeDragGestureHandler2D::Update::Type::End:
                            std::cout << "Drag end" << std::endl;
                            break;
                        default:
                            break;
                    }

                    std::ostringstream ss;
                    ss << "Delta: " << update.delta.x << ", " << update.delta.y << std::endl;
                    textRenderer->SetText(ss.str());
                    // BUG: textRenderer->SizeToFit();
                };
            })
            .PopToRoot()
            .And("Button")
            .With<SpriteRenderer>(buttonTexture)
            .RectCollider({200, 50})
            .With<ButtonControl>()
            .SetWorldPosition({0, -90, 0})
            .ModifyLatest<ButtonControl>([=](ButtonControl& button) {
                button.onPressFunc = [](auto&) {
                    std::cout << "Press" << std::endl;
                };
                button.SetWorldSize({200, 50, 0});
                button.SetOnControlUpdateFunc([](UIControl2D& control) {
                    auto spriteRenderer = control.owner->TypeComponent<SpriteRenderer>();
                    GUARD(spriteRenderer)
                     spriteRenderer->SetColor(((ButtonControl*)&control)->IsPressed() ? Themes::fruit.ThemeColor("blue") : Color::white);
                });
            });
    }
};
