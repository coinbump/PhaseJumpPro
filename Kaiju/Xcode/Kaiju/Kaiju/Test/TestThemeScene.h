#pragma once

#include <PhaseJump/PhaseJump.h>

using namespace PJ;

class TestThemeScene : public Scene {
public:
    TestThemeScene() {
    }

    void LoadInto(WorldNode& root) {
        root.name = "TestThemeScene";

        auto& camera = root.AddComponent<OrthoCamera>();
        camera.owner->AddComponent<SimpleRaycaster2D>();

        for (auto& color : Themes::fruit.Colors()) {
            root.AddNode("Theme color")
                .AddCircle(20, color.second);
        }

        root.AddComponent<HFlow>(50)
            .ApplyLayout();
    }
};
