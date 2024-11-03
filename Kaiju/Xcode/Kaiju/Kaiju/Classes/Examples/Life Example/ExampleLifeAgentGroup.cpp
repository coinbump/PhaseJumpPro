#include "ExampleLifeAgentGroup.h"
#include "ExampleLifeMatrixRenderer.h"

using namespace std;
using namespace PJ;
using namespace Example::Life;

Example::Life::AgentGroup::AgentGroup(MatrixRenderer* matrixView) :
    Base(matrixView->matrix.Count()),
    matrixView(matrixView) {
    stepTime = 0.1f;
}

void Example::Life::AgentGroup::PostStep() {
    Base::PostStep();

    RunAgentsAction([](auto& agent) { agent.core.isAlive = agent.core.nextIsAlive; });
    matrixView->PostStep();
}

int Example::Life::AgentGroup::LiveNeighborsCountFor(Agent const& agent) {
    auto result = 0;

    for (int x = agent.core.location.x - 1; x <= agent.core.location.x + 1; x++) {
        for (int y = agent.core.location.y - 1; y <= agent.core.location.y + 1; y++) {
            Vector2Int location(x, y);
            GUARD_CONTINUE(location != agent.core.location)
            GUARD_CONTINUE(matrixView->matrix.IsValidLocation(location))

            if (matrixView->matrix.CellAt(location)->core.isAlive) {
                result++;
            }
        }
    }

    return result;
}

// MARK: ExampleLifeAgents

Example::Life::Agent::OnStepFunc LifeAgents::MakeOnStepFunc() {
    return [](auto& agent) {
        auto group = static_cast<AgentGroup*>(agent.group);
        GUARD(group)

        auto liveNeighborsCount = group->LiveNeighborsCountFor(agent);

        if (agent.core.isAlive) {
            switch (liveNeighborsCount) {
            case 2:
            case 3:
                break;
            default:
                agent.core.nextIsAlive = false;
                break;
            }
        } else {
            switch (liveNeighborsCount) {
            case 3:
                agent.core.nextIsAlive = true;
                break;
            default:
                break;
            }
        }
    };
}
