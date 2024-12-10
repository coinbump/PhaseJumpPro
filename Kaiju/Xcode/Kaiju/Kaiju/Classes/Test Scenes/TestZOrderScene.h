#pragma once

#include "SDKIncludes.h"

using namespace PJ;

class TestZOrderScene : public Scene {
public:
    TestZOrderScene() {}

    void LoadInto(WorldNode& root) override {
        root.name = "TestZOrderScene";

        root.With<OrthoCamera>();

        auto& n1 = root.And();
        auto& n11 = n1.AddNodeAt(Vector3(-100, 0, 0), "n11");
        auto& n111 = n11.AddNodeAt(Vector3(0, 0, 0), "n111");
        auto& n12 = n1.AddNodeAt(Vector3(25, 0, 0), "n12");
        auto& n121 = n12.AddNodeAt(Vector3(-50, 0, 0), "n121");
        auto& n122 = n12.AddNodeAt(Vector3(50, 0, 0), "122");

        n1.With<ColorRenderer>(ColorRenderer::Config{ .color = Color::yellow.WithAlpha(.7),
                                                      .worldSize = Vector2(400, 400) })
            .model.zIndex = 1;
        n11.With<ColorRenderer>(ColorRenderer::Config{
            .color = Color::white.WithColorsMult(.3).WithAlpha(.7), .worldSize = Vector2(200, 300) }
        );
        n111.With<ColorRenderer>(Color::green.WithAlpha(.7f), Vector2(150, 100));
        n12.With<ColorRenderer>(Color::red.WithAlpha(.7f), Vector2(200, 300));
        n121.With<ColorRenderer>(Color::white.WithAlpha(.7f), Vector2(50, 50));
        n122.With<ColorRenderer>(Color::white.WithAlpha(.7f), Vector2(50, 50)).model.zIndex = 3;
    }
};
