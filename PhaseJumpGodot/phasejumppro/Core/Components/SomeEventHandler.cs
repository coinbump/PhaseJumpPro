using System;
using Godot;
using PJ;

/*
RATING: 5 stars
Simple type
CODE REVIEW: 6/10/23
*/
namespace PJ
{
    /// <summary>
    /// A component that listens to events
    /// </summary>
    public abstract partial class SomeEventHandler : WorldComponent, SomeListener
    {
        public abstract void OnEvent(Event _event);
    }
}
