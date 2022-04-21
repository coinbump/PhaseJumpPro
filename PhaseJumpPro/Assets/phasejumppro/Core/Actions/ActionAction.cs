using System;

namespace PJ
{
    /// <summary>
    /// Uses a System.Action closure for the action class
    /// </summary>
    public class ActionAction : SomeAction
    {
        // CAREFUL: be aware of reference cycles for actions that capture
        public Action action;

        public ActionAction(Action action)
        {
            this.action = action;
        }

        public override void Go()
        {
            if (null == action) { return; }
            action();
        }
    }
}
