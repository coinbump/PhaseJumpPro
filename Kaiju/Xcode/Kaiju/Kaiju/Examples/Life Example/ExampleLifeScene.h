#pragma once

#include "ExampleLifeAgent.h"
#include "ExampleLifeMatrixRenderer.h"
#include "ExampleLifeAgentGroup.h"
#include <PhaseJump/PhaseJump.h>

using namespace PJ;

/// "Life" example using the common rules for a basic cellular automata simulation
class ExampleLifeScene : public Scene {
public:
    SP<BezierPathLayout> bezierLayout;

    ExampleLifeScene() {
    }

    void LoadInto(WorldNode& root) {
        root.name = "ExampleLifeScene";

        auto& camera = root.AddComponent<OrthoCamera>();
        camera.owner->AddComponent<SimpleRaycaster2D>();

        root.AddNode("Life Simulation")
            .AddComponent<ExampleLifeMatrixRenderer>();
    }
};
