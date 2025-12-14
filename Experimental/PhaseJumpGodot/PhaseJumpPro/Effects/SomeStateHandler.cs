using System.Collections.Generic;
using System;
using Godot;

/*
RATING: 5 stars
Simple interface
GODOT CODE REVIEW: 12/30/23
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Handles state changes
    /// </summary>
    public abstract partial class SomeStateHandler<StateType> : WorldNode
    {
        public abstract void OnStateChange(StateType state);
    }
}
