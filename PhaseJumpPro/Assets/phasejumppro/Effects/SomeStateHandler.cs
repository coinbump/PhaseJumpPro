using System.Collections.Generic;
using System;
using UnityEngine;

/*
RATING: 5 stars
Simple interface
CODE REVIEW: 5/8/23
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Handles state changes
    /// </summary>
    public abstract class SomeStateHandler<StateType> : WorldComponent
    {
        public abstract void OnStateChange(StateType state);
    }
}
