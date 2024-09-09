#pragma once

#include <PhaseJump/PhaseJump.h>

using namespace PJ;

class TestUIScene : public Scene {
public:
    SP<BezierPathLayout> bezierLayout;

    VectorList<Vector3> controlPoints{{-300, 0, 0}, {-150, 300, 0}, {300, 300, 0}, {300, 450, 0}};

    TestUIScene() {
    }

    void LoadInto(WorldNode& root) {
        root.name = "TestUIScene";

        auto& camera = root.AddComponent<OrthoCamera>();
        camera.owner->AddComponent<SimpleRaycaster2D>();

        auto& dragGestureNode = root.AddNode("Drag Gesture")
            .AddCircle(20)
            .AddCircleCollider(20);

        auto& dragGestureHandler = dragGestureNode
            .AddComponent<SomeDragGestureHandler2D>();

        auto world = root.World();

        auto font = DCAST<Font>(world->loadedResources->map["font"]["ArialBlack-32"].resource);
        GUARD(font)

        auto textNode = root.AddNodePtr("Label");
        auto textRenderer = textNode->AddComponentPtr<TextRenderer>(font, "Delta:", Vector2(400, 400));
        textNode->SetLocalPosition(Vector3{0, -30, 0});

        dragGestureHandler.onDragGestureUpdateFunc = [=](SomeDragGestureHandler2D::Update update) {
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

        auto& button = root.AddNode()
            .AddComponent<ButtonControl>();

        auto buttonTexture = DCAST<GLTexture>(root.World()->loadedResources->map["texture"]["example-button-normal"].resource);
        GUARD(buttonTexture)
        auto spriteRenderer = button.owner->AddComponentPtr<SpriteRenderer>(buttonTexture);
        button.owner->AddRectCollider({(float)buttonTexture->Size().x, (float)buttonTexture->Size().y});
        button.onPressFunc = [](auto&) {
            std::cout << "Press" << std::endl;
        };
        button.owner->SetWorldPosition(Vector3(0, -90, 0));
        button.SetOnControlUpdateFunc([=](auto& control) {
            auto& button = *(static_cast<ButtonControl*>(&control));
            spriteRenderer->SetColor(button.IsPressed() ? Themes::fruit.ThemeColor("blue") : Color::white);
        });
    }
};
