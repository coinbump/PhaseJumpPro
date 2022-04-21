using System;

namespace PJ
{
    /// <summary>
    /// An action is an executable piece of code
    /// </summary>
    public abstract class SomeAction
    {
        public Tags parameters = new Tags();

        public abstract void Go();
    }
}
