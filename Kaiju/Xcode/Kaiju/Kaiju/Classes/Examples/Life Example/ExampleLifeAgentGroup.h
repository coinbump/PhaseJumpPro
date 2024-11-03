#pragma once

#include "ExampleLifeAgent.h"
#include "SDKIncludes.h"

using namespace PJ;

namespace Example {
    namespace Life {
        class MatrixRenderer;

        /// Agent group for the life simulation
        class AgentGroup : public PJ::AgentGroup<Agent> {
        public:
            using Base = PJ::AgentGroup<Agent>;

            /// Matrix owner of the agent group
            MatrixRenderer* matrixView{};

            AgentGroup(MatrixRenderer* matrixView);

            void PostStep() override;

            int LiveNeighborsCountFor(Agent const& agent);
        };

        // MARK: ExampleLifeAgents

        namespace LifeAgents {
            /// Handles simulation step rules
            extern Agent::OnStepFunc MakeOnStepFunc();
        }; // namespace LifeAgents
    } // namespace Life
} // namespace Example
