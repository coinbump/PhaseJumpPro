#pragma once

#include "ExampleLifeAgent.h"
#include "ExampleLifeAgentGroup.h"
#include "ExampleLifeMatrixRenderer.h"
#include "SDKIncludes.h"

using namespace PJ;

namespace Example {
    namespace Life {
        /// "Life" example using the common rules for a basic cellular automata simulation
        class Scene : public PJ::Scene {
        public:
            Scene() {}

            void LoadInto(WorldNode& root) override {
                QB(root)
                    .Named("ExampleLifeScene")
                    .OrthoStandard(Color::gray)
                    .And("Life Simulation")
                    .With<MatrixRenderer>();
            }
        };
    } // namespace Life
} // namespace Example
