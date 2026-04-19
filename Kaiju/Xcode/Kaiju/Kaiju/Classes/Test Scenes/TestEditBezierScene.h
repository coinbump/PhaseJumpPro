#pragma once

#include "SDKIncludes.h"

using namespace PJ;

class TestEditBezierScene : public Scene {
public:
    VectorList<Vector3> controlPoints{
        { -300, 0, 0 }, { -150, 300, 0 }, { 300, 300, 0 }, { 300, 450, 0 }
    };

    TestEditBezierScene() {}

    void LoadInto(WorldNode& root) override {
        root.SetName("TestEditBezierScene");

        QB(root).OrthoStandard();

        auto& bezierNode = root.And("bezier");
        QB qb(bezierNode);

        qb.With<ColorRenderer>(ColorRenderer::Config{ .color = Color::blue })
            .ModifyLatest<ColorRenderer>([this](auto& colorRenderer) {
                colorRenderer.core.SetBuildMeshFunc([this](auto& model) {
                    BezierPath bezierPath;
                    bezierPath.SetControlPoints(controlPoints);
                    BezierFrameMeshBuilder meshBuilder({ .bezierPath = bezierPath,
                                                         .strokeWidth = 20 });
                    return meshBuilder.BuildMesh();
                });
            });

        for (int i = 0; i < controlPoints.size(); i++) {
            QB(root.And("Drag handle"))
                .Circle(10, Themes::fruit.ThemeColor("red"))
                .CircleCollider(10)
                .ModifyLatest<CircleCollider2D>([](auto& c) {
                    // Just test code for .Modify
                    // c.radius = 20;
                })
                .Drag([=, this](auto& handler, auto position) {
                    controlPoints[i] = position;

                    WorldNode* parent = handler.owner->Parent();
                    GUARD(parent)

                    auto bezier = (*parent)["bezier"];
                    GUARD(bezier)

                    auto colorRenderer = bezier->TypeComponent<ColorRenderer>();
                    GUARD(colorRenderer)

                    colorRenderer->core.model.SetMeshNeedsBuild();
                })
                .SetWorldPosition(controlPoints[i]);
        }
    }
};
