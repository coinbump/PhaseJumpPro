#include "ExampleLifeAgentGroup.h"
#include "ExampleLifeMatrixRenderer.h"

using namespace std;
using namespace PJ;
using namespace ExampleLife;

ExampleLife::AgentGroup::AgentGroup(MatrixRenderer* matrixView) : Base(matrixView->matrix.Count()), matrixView(matrixView)
{
    stepTime = 0.1f;
}

void ExampleLife::AgentGroup::PostStep()
{
    Base::PostStep();
    matrixView->PostStep();
    RunAgentsAction([this](auto& agent) { UpdateMatrixForAgent((Agent const&)agent); });
}

int ExampleLife::AgentGroup::LiveNeighborsCountFor(Agent const& agent) {
    auto result = 0;

    for (int x = agent.core.location.x - 1; x <= agent.core.location.x + 1; x++)
    {
        for (int y = agent.core.location.y - 1; y <= agent.core.location.y + 1; y++)
        {
            Vector2Int location(x, y);
            GUARD_CONTINUE(location != agent.core.location)
            GUARD_CONTINUE(matrixView->matrix.IsValidLocation(location))

            if (matrixView->matrix.CellAt(location) == MatrixRenderer::CellState::Alive) {
                result++;
            }
        }
    }

    return result;
}

void ExampleLife::AgentGroup::UpdateMatrixForAgent(Agent const& agent)
{
    GUARD(matrixView)
    matrixView->UpdateMatrixForAgent(agent);
}

// MARK: ExampleLifeAgents

ExampleLife::Agent::OnStepFunc ExampleLifeAgents::MakeOnStepFunc() {
    return [](auto& agent) {
        auto group = static_cast<AgentGroup*>(agent.group);
        GUARD(group)

        auto liveNeighborsCount = group->LiveNeighborsCountFor(agent);

        if (agent.core.isAlive)
        {
            switch (liveNeighborsCount)
            {
                case 2:
                case 3:
                    break;
                default:
                    agent.core.isAlive = false;
                    break;
            }
        }
        else
        {
            switch (liveNeighborsCount)
            {
                case 3:
                    agent.core.isAlive = true;
                    break;
                default:
                    break;
            }
        }
    };
}
