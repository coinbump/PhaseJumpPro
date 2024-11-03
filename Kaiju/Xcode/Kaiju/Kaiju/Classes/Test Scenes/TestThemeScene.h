#pragma once

#include "SDKIncludes.h"

using namespace PJ;

class TestThemeScene : public Scene {
public:
    TestThemeScene() {}

    void LoadInto(WorldNode& root) {
        QB qb(root);

        qb.Named("TestThemeScene")
            .With<OrthoCamera>()
            .With<SimpleRaycaster2D>()
            .And("Layout")
            .With<HFlow>(50);

        for (auto& color : Themes::fruit.Colors()) {
            qb.And("Theme color").Circle(20, color.second).Pop();
        }

        qb.ModifyAll<HFlow>([](HFlow& c, auto index) { c.LayoutIfNeeded(); });
    }
};
