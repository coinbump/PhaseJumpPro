#pragma once

#include "SDKIncludes.h"

using namespace PJ;

class TestSteeringScene : public Scene {
public:
    TestSteeringScene() {}

    void LoadInto(WorldNode& root) override {
        root.name = "TestSteeringScene";

        StandardRandom random;

        QB(root).OrthoStandard().Repeat(20, [&](QuickBuild& qb) {
            qb.And("Steering")
                .Texture("heart-full")
                .With<DirectionKSteering2D>(random.IntValue(40, 60), 100, 800)
                .With<RotateKSteering2D>(Angle::DegreesAngle(random.IntValue(-45, 45)))
                .Pop();
        });
    }
};
