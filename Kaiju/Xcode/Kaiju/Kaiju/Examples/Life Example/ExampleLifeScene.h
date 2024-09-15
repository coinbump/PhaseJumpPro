#pragma once

#if DEVELOPMENT
#include <PhaseJump-Dev/PhaseJump-Dev.h>
#else
#include <PhaseJump/PhaseJump.h>
#endif
#include "ExampleLifeAgent.h"
#include "ExampleLifeMatrixRenderer.h"
#include "ExampleLifeAgentGroup.h"

using namespace PJ;

namespace ExampleLife {
    /// "Life" example using the common rules for a basic cellular automata simulation
    class Scene : public PJ::Scene {
    public:
        Scene() {
        }

        void LoadInto(WorldNode& root) {
            QB(root)
                .Named("ExampleLifeScene")
                .With<OrthoCamera>()
                .With<SimpleRaycaster2D>()
                .And("Life Simulation")
                .With<MatrixRenderer>();
        }
    };
}
