#pragma once

#if DEVELOPMENT
#include <PhaseJump-Dev/PhaseJump-Dev.h>
#else
#include <PhaseJump/PhaseJump.h>
#endif
#include "ExampleLifeAgent.h"

using namespace PJ;

namespace ExampleLife {
    class MatrixRenderer;

    /// Agent group for the life simulation
    class AgentGroup : public PJ::AgentGroup<Agent>
    {
    public:
        using Base = PJ::AgentGroup<Agent>;

        /// Matrix owner of the agent group
        MatrixRenderer* matrixView = nullptr;

        AgentGroup(MatrixRenderer* matrixView);

        void PostStep() override;

        int LiveNeighborsCountFor(Agent const& agent);

    protected:
        void UpdateMatrixForAgent(Agent const& agent);
    };

    // MARK: ExampleLifeAgents

    namespace ExampleLifeAgents {
        /// Handles simulation step rules
        extern Agent::OnStepFunc MakeOnStepFunc();
    };
}
