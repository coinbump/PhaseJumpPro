using System.Collections.Generic;
using System;
using UnityEngine;

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
