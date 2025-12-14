using System;

namespace PJ
{
    /// <summary>
    /// A selector selects a result from a collection of choices
    /// </summary>
    public abstract class SomeSelector<Output>
    {
        public abstract Output Select();
    }
}
