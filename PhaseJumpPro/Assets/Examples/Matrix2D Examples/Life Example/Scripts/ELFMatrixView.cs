using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

/// <summary>
/// Runs the simulation and updates the matrix board based on cell states
/// </summary>
public class ELFMatrixView : MatrixBoardView2D
{
    /// <summary>
    /// Object that will be placed in the cell for "live" cells (empty otherwise)
    /// </summary>
    public MatrixNode2D cellObject;

    protected SimSystem simSystem = new();
    protected float updateSystemCountdown;

    public ELFMatrixView()
    {
    }

    protected override void Awake()
    {
        base.Awake();

        // Create a group for the agents
        var agentGroup = new ELFSimAgentGroup(this);
        simSystem.Add(agentGroup);

        // Populate the simulation with agents
        for (int y = 0; y < matrixSize.y; y++)
        {
            for (int x = 0; x < matrixSize.x; x++)
            {
                var agent = new ELFSimAgent(new Vector2Int(x, y));

                agentGroup.Add(agent);

                // Add some random live cells
                if (new UnityRandom().Value < 0.2f)
                {
                    agent.IsAlive = true;
                    UpdateMatrixForAgent(agent);
                }
            }
        }
    }

    public override void OnUpdate(TimeSlice time)
    {
        base.OnUpdate(time);

        // After user taps in view to change cell, pause the simulation for N seconds
        if (updateSystemCountdown > 0)
        {
            updateSystemCountdown -= time.delta;
            if (updateSystemCountdown > 0)
            {
                return;
            }
        }
        simSystem.OnUpdate(time);
    }

    public void UpdateMatrixForAgent(ELFSimAgent agent)
    {
        if (agent.IsAlive)
        {
            var location = agent.location;
            if (!board.IsCellOccupied(location))
            {
                var cellPiece = new MatrixPiece(new Vector2Int(1, 1));
                var cellObject = Instantiate(this.cellObject, Vector3.zero, Quaternion.identity);
                Put(cellObject, agent.location);
            }
        }
        else
        {
            RemoveAt(agent.location);
        }
    }

    protected ELFSimAgent AgentAt(Vector2Int location)
    {
        var agentGroup = simSystem.AgentGroups[0] as ELFSimAgentGroup;

        // This is inefficient, but ok for a simple demo
        for (int i = 0; i < agentGroup.Agents.Length; i++)
        {
            var agent = agentGroup.Agents[i] as ELFSimAgent;
            if (null == agent) { break; }

            if (agent.location == location)
            {
                return agent;
            }
        }

        return null;
    }

    public override void OnPointerClickEvent(PointerClickUIEvent<LocalPosition> _event)
    {
        var localPressPosition = _event.pressPosition;
        var cellHit = LocationAtLocalPosition(localPressPosition.Position);

        if (cellHit.value == null) { return; }

        var agent = AgentAt(cellHit.value);
        if (null == agent) { return; }

        agent.IsAlive = !agent.IsAlive;
        UpdateMatrixForAgent(agent);
        updateSystemCountdown = 3.0f;
    }
}
