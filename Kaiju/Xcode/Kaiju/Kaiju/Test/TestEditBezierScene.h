#pragma once

#include <PhaseJump/PhaseJump.h>

using namespace PJ;

class TestEditBezierScene : public Scene {
public:
    SP<BezierPathLayout> bezierLayout;

    VectorList<Vector3> controlPoints{{-300, 0, 0}, {-150, 300, 0}, {300, 300, 0}, {300, 450, 0}};

    TestEditBezierScene() {
    }

    void LoadInto(WorldNode& root) {
        root.name = "TestEditBezierScene";

        auto& camera = root.AddComponent<OrthoCamera>();
        camera.owner->AddComponent<SimpleRaycaster2D>();

        for (int i = 0; i < controlPoints.size(); i++)  {
            root.AddNode("Drag handle")
                .AddCircle(10, Themes::fruit.ThemeColor("red"))
                // FUTURE: support circle colliders
                .AddCircleCollider(10)
                .AddDrag([=, this](DragHandler2D& handler) {
                    this->bezierLayout->SetControlPoint(i, handler.owner->transform.WorldPosition());
                })
                .SetWorldPosition(controlPoints[i]);
        }

        auto& bezierNode = root.AddNode();
        bezierLayout = bezierNode.AddComponentPtr<BezierPathLayout>();
        bezierLayout->SetControlPoints(controlPoints);

        for (int i = 0; i < 50; i++) {
            bezierLayout->owner->AddNode("Bezier point")
                .AddCircle(2.5f, Themes::fruit.ThemeColor("blue"));
        }
        bezierLayout->ApplyLayout();
    }
};
