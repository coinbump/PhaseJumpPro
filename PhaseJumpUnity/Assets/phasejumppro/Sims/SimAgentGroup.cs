using System;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Handles updates and collection logic for a group of agents
    /// </summary>
    public class SimAgentGroup : Updatable
    {
        /// <summary>
        /// System that owns this group
        /// </summary>
        public WeakReference<SimSystem> system;

        /// <summary>
        /// (OPTIONAL) For step-based simulations, the time value between steps (0 if steps are disabled)
        /// </summary>
        public float stepTime;

        protected float stepTimer;
        protected SomeSimAgent[] agents;
        protected int firstInactiveAgentIndex;
        protected int lastActiveAgentIndex = -1;

        public virtual bool IsFinished => false;

        // FUTURE: support reusing inactive agents if needed.
        public int FirstInactiveAgentIndex => firstInactiveAgentIndex;
        public SomeSimAgent[] Agents => agents;

        public SimAgentGroup(int size = 100)
        {
            agents = new SomeSimAgent[size];
        }

        public virtual void Add(SomeSimAgent agent)
        {
            // Grow collection as needed (or if adding this agent will fill it)
            if ((firstInactiveAgentIndex + 1) >= agents.Length)
            {
                Array.Resize<SomeSimAgent>(ref agents, agents.Length * 2);
            }

            agents[firstInactiveAgentIndex] = agent;
            agent.IsActive = true;
            agent.index = firstInactiveAgentIndex;
            agent.group = new WeakReference<SimAgentGroup>(this);
            lastActiveAgentIndex = Math.Max(lastActiveAgentIndex, agent.index);

            for (int i = firstInactiveAgentIndex + 1; i < agents.Length; i++)
            {
                if (null == agents[i] || !agents[i].IsActive)
                {
                    firstInactiveAgentIndex = i;
                    break;
                }
            }
        }

        public virtual void Remove(SomeSimAgent agent)
        {
            RemoveAt(agent.index);
        }

        public virtual void RemoveAt(int index)
        {
            if (index < 0 || index >= agents.Length) { return; }

            agents[index].IsActive = false;
            agents[index].index = -1;
            firstInactiveAgentIndex = Math.Min(firstInactiveAgentIndex, index);

            // Update the new final end of the range of agents
            if (lastActiveAgentIndex == index)
            {
                var newLastActiveAgentIndex = -1;
                for (int i = index - 1; i >= 0; i--)
                {
                    if (agents[i] != null && agents[i].IsActive)
                    {
                        newLastActiveAgentIndex = i;
                        break;
                    }
                }
                lastActiveAgentIndex = newLastActiveAgentIndex;
            }
        }

        public virtual void AgentsAction(Action<SomeSimAgent> action)
        {
            var groupSize = agents.Length;
            for (int i = 0; (i < groupSize && i <= lastActiveAgentIndex); i++)
            {
                var agent = agents[i];
                if (null == agent || !agent.IsActive) { continue; }

                action(agent);
            }
        }

        public virtual void OnUpdate(TimeSlice time)
        {
            // Give all agents update delta
            AgentsAction((agent) => agent.OnUpdate(time));

            // For step-based simulation give all agents steps
            if (stepTime > 0)
            {
                stepTimer += time.delta;
                while (stepTimer > stepTime)
                {
                    AgentsAction((agent) => agent.OnStep());
                    PostStep();
                    stepTimer -= stepTime;
                }
            }
        }

        /// <summary>
        /// (OPTIONAL) After each step, do any needed logic
        /// </summary>
        protected virtual void PostStep() { }
    }
}
