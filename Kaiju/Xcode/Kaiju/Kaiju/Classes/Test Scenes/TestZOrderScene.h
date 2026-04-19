#pragma once

#include "SDKIncludes.h"

using namespace PJ;

class TestZOrderScene : public Scene {
public:
    TestZOrderScene() {}

    void LoadInto(WorldNode& root) override {
        root.SetName("TestZOrderScene");

        QB(root).OrthoStandard();

        auto& n1 = root.And();
        auto& n11 = n1.AddNodeAt(Vector3(-100, 0, 0), WorldNode::Config{ .name = "n11" });
        auto& n111 = n11.AddNodeAt(Vector3(0, 0, 0), WorldNode::Config{ .name = "n111" });
        auto& n12 = n1.AddNodeAt(Vector3(25, 0, 0), WorldNode::Config{ .name = "n12" });
        auto& n121 = n12.AddNodeAt(Vector3(-50, 0, 0), WorldNode::Config{ .name = "n121" });
        auto& n122 = n12.AddNodeAt(Vector3(50, 0, 0), WorldNode::Config{ .name = "122" });

        n1.With<ColorRenderer>(ColorRenderer::Config{ .color = Color::yellow.WithAlpha(.7),
                                                      .worldSize = Vector2(400, 400) })
            .core.model.zIndex = 1;
        n11.With<ColorRenderer>(ColorRenderer::Config{
            .color = Color::white.WithColorsMult(.3).WithAlpha(.7), .worldSize = Vector2(200, 300) }
        );
        n111.With<ColorRenderer>(ColorRenderer::Config{ .color = Color::green.WithAlpha(.7f),
                                                        .worldSize = Vector2(150, 100) });
        n12.With<ColorRenderer>(ColorRenderer::Config{ .color = Color::red.WithAlpha(.7f),
                                                       .worldSize = Vector2(200, 300) });
        n121.With<ColorRenderer>(ColorRenderer::Config{ .color = Color::white.WithAlpha(.7f),
                                                        .worldSize = Vector2(50, 50) });
        n122.With<ColorRenderer>(ColorRenderer::Config{ .color = Color::white.WithAlpha(.7f),
                                                        .worldSize = Vector2(50, 50) })
            .core.model.zIndex = 3;
    }
};
