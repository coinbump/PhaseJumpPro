#pragma once

#include "SDKIncludes.h"

using namespace PJ;

class TestEditBezierScene : public Scene {
public:
    SP<BezierPathLayout> bezierLayout;

    VectorList<Vector3> controlPoints{
        { -300, 0, 0 }, { -150, 300, 0 }, { 300, 300, 0 }, { 300, 450, 0 }
    };

    TestEditBezierScene() {}

    void LoadInto(WorldNode& root) override {
        root.name = "TestEditBezierScene";

        auto& camera = root.With<OrthoCamera>();
        camera.owner->With<SimpleRaycaster2D>();

        auto& bezierNode = root.And();
        bezierLayout = bezierNode.WithPtr<BezierPathLayout>();
        bezierLayout->SetControlPoints(controlPoints);

        QB qb(*bezierLayout->owner);
        size_t count = 500;

        for (int i = 0; i < count; i++) {
            qb++.Circle(20.0f, Themes::fruit.ThemeColor("blue"))--;
        }

        qb.ModifyAll<ColorRenderer>([=](ColorRenderer& c, auto index) {
            auto color = c.GetColor();
            color.r = ((float)index / (float)count) * 0.7f;
            c.SetColor(color);
        });

        bezierLayout->ApplyLayout();

        for (int i = 0; i < controlPoints.size(); i++) {
            QB(root.And("Drag handle"))
                .Circle(10, Themes::fruit.ThemeColor("red"))
                .CircleCollider(10)
                .ModifyLatest<CircleCollider2D>([](auto& c) {
                    // Just test code for .Modify
                    // c.radius = 20;
                })
                .Drag([=, this](auto& handler) {
                    this->bezierLayout->SetControlPoint(
                        i, handler.owner->transform.WorldPosition()
                    );
                })
                .SetWorldPosition(controlPoints[i]);
        }
    }
};
