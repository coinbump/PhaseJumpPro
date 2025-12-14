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
            Vector3 worldSize{ 1200, 1200, 0 };

            Scene() {}

            void LoadInto(WorldNode& root) override {
                QB(root)
                    .Named("ExampleLifeScene")
                    .OrthoStandard(QuickBuilder::OrthoStandardConfig{ .clearColor = Color::gray })
                    .And("Life Simulation")
                    .With<MatrixRenderer>(worldSize)
                    .ScaleWithWindow(worldSize, 0.8f);
            }
        };
    } // namespace Life
} // namespace Example
