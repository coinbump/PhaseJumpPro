#pragma once

#include <PhaseJump/PhaseJump.h>

using namespace PJ;

class TestZOrderScene : public Scene {
public:
    SP<RenderMaterial> heartMaterial;

    TestZOrderScene() {
    }

    void LoadInto(WorldNode& root) {
        root.name = "TestZOrderScene";

        root.AddComponent<OrthoCamera>();

        auto& n1 = root.AddNode();
        auto& n11 = n1.AddNodeAt(Vector3(-100, 0, 0), "n11");
        auto& n111 = n11.AddNodeAt(Vector3(0, 0, 0), "n111");
        auto& n12 = n1.AddNodeAt(Vector3(25, 0, 0), "n12");
        auto& n121 = n12.AddNodeAt(Vector3(-50, 0, 0), "n121");
        auto& n122 = n12.AddNodeAt(Vector3(50, 0, 0), "122");

        n1.AddComponent<ColorRenderer>(Color::yellow.WithAlpha(.7), Vector2(400, 400)).model.zIndex = 1;
        n11.AddComponent<ColorRenderer>(Color::white.WithColorsMult(.3).WithAlpha(.7), Vector2(200, 300));
        n111.AddComponent<ColorRenderer>(Color::green.WithAlpha(.7), Vector2(150, 100));
        n12.AddComponent<ColorRenderer>(Color::red.WithAlpha(.7), Vector2(200, 300));
        n121.AddComponent<ColorRenderer>(Color::white.WithAlpha(.7), Vector2(50, 50));
        n122.AddComponent<ColorRenderer>(Color::white.WithAlpha(.7), Vector2(50, 50)).model.zIndex = 3;
    }
};
