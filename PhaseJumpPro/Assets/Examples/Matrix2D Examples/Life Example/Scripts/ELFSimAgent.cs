using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

/// <summary>
/// An agent in the life simulation that handles the alive/dead switch rules
/// </summary>
public class ELFSimAgent : SomeSimAgent
{
    protected bool isAlive;
    public Vector2Int location;

    public bool IsAlive
    {
        get => isAlive;
        set => isAlive = value;
    }

    public ELFSimAgent(Vector2Int location)
    {
        this.location = location;
    }

    public override void OnStep()
    {
        base.OnStep();

        if (group.TryGetTarget(out SimAgentGroup _group))
        {
            var agentGroup = _group as ELFSimAgentGroup;
            var liveNeighborsCount = agentGroup.LiveNeighborsCountFor(this);

            if (isAlive)
            {
                switch (liveNeighborsCount)
                {
                    case 2:
                    case 3:
                        break;
                    default:
                        isAlive = false;
                        break;
                }
            }
            else
            {
                switch (liveNeighborsCount)
                {
                    case 3:
                        isAlive = true;
                        break;
                    default:
                        break;
                }
            }
        }
    }
}
