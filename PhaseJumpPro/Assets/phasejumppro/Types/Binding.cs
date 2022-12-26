using PJ;
using System;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple abstract class
 * CODE REVIEW: 11/26/22
 * PORTED TO: C++
 */
namespace PJ
{
    public class Binding<T> : SomeBinding<T>
    {
        Func<T> getFunc;
        Action<T> setAction;

        public Binding(Func<T> getFunc, Action<T> setAction)
        {
            this.getFunc = getFunc;
            this.setAction = setAction;
        }

        public override T Value
        {
            get => getFunc();
            set => setAction(value);
        }
    }

    /// <summary>
    /// Set-only binding (one way)
    /// </summary>
    public class SetBinding<T> : SomeSetBinding<T>
    {
        Action<T> setAction;

        public SetBinding(Action<T> setAction)
        {
            this.setAction = setAction;
        }

        public override T Value
        {
            set => setAction(value);
        }
    }
}
