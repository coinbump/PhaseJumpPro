using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Simple utility
CODE REVIEW: 1/14/23
*/
namespace PJ
{
    /// <summary>
    /// A simulation contains agents, each agent is responsible for its own localized logic
    /// For convenience, agents can be divided into Agent Groups
    /// </summary>
    public class SimSystem : Updatable
    {
        protected List<SimAgentGroup> agentGroups = new();

        public List<SimAgentGroup> AgentGroups => agentGroups;

        public bool IsFinished => false;

        public void OnUpdate(TimeSlice time)
        {
            foreach (var agentGroup in agentGroups)
            {
                agentGroup.OnUpdate(time);
            }
        }

        public void Add(SimAgentGroup agentGroup)
        {
            agentGroup.system = new WeakReference<SimSystem>(this);
            agentGroups.Add(agentGroup);
        }
    }
}
