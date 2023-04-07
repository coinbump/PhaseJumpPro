using System;
using UnityEngine;
using PJ;

namespace PJ
{
    /// <summary>
    /// Responds to events (similar to SomeListener)
    /// </summary>
    public abstract class SomeEventHandler : WorldComponent, SomeListener
    {
        public abstract void OnEvent(Event _event);
    }
}
