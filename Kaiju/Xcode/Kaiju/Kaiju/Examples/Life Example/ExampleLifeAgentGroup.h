#pragma once

#include <PhaseJump/PhaseJump.h>
#include "ExampleLifeAgent.h"

using namespace PJ;

class ExampleLifeMatrixRenderer;

/// Agent group for the life simulation
class ExampleLifeAgentGroup : public AgentGroup<ExampleLifeAgent>
{
public:
    using Base = AgentGroup<ExampleLifeAgent>;

    /// Matrix owner of the agent group
    ExampleLifeMatrixRenderer* matrixView = nullptr;

    ExampleLifeAgentGroup(ExampleLifeMatrixRenderer* matrixView);

    void PostStep() override;

    int LiveNeighborsCountFor(ExampleLifeAgent const& agent);

protected:
    void UpdateMatrixForAgent(ExampleLifeAgent const& agent);
};

// MARK: ExampleLifeAgents

namespace ExampleLifeAgents {
    /// Handles simulation step rules
    extern ExampleLifeAgent::OnStepFunc MakeOnStepFunc();
};
