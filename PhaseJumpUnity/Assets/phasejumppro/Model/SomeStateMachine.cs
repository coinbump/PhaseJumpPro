using System;
using PJ;

/*
 * RATING: 5 stars
 * Simple interface
 * CODE REVIEW: 12/11/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Some state machine.
    /// A state machine is a graph of states, with transition edges
    /// </summary>
    public interface SomeStateMachine<StateType>
    {
        public StateType State { get; }
    }
}
