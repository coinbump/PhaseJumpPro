using System;
using Godot;

/*
RATING: 5 stars
Simple type
CODE REVIEW: 12/17/22
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Trigger an action at the keyframe's time
    /// </summary>
    public class ActionKeyframe : SomeKeyframe
    {
        protected Action action;

        public ActionKeyframe(float time, Action action) : base(time)
        {
            this.action = action;
        }

        public void Go()
        {
            action();
        }
    }
}
