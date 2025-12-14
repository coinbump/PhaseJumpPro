using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

/// <summary>
/// Handles group-related logic for life agents and communicates changes to the
/// matrix view
/// </summary>
public class ELFSimAgentGroup : SimAgentGroup
{
    /// <summary>
    /// Matrix owner of the agent group
    /// </summary>
    public ELFMatrixView matrixView;

    public ELFSimAgentGroup(ELFMatrixView matrixView)
    {
        this.matrixView = matrixView;
        stepTime = 0.5f;
    }

    protected override void PostStep()
    {
        base.PostStep();
        AgentsAction((agent) => UpdateMatrixForAgent(agent as ELFSimAgent));
    }

    public int LiveNeighborsCountFor(ELFSimAgent agent)
    {
        var result = 0;

        for (int x = agent.location.x - 1; x <= agent.location.x + 1; x++)
        {
            for (int y = agent.location.y - 1; y <= agent.location.y + 1; y++)
            {
                var location = new Vector2Int(x, y);
                if (location == agent.location) { continue; }
                if (!matrixView.board.IsValidLocation(location)) { continue; }

                if (matrixView.board.IsCellOccupied(location))
                {
                    result++;
                }
            }
        }

        return result;
    }

    protected void UpdateMatrixForAgent(ELFSimAgent agent)
    {
        matrixView.UpdateMatrixForAgent(agent);
    }
}
