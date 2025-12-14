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
    /// <summary>
    /// Type erasure interface
    /// </summary>
    public abstract class AnyBinding
    {
    }

    /// <summary>
    /// Wrapper used to alter the attribute of an object
    /// </summary>
    public abstract class SomeBinding<T> : AnyBinding
    {
        public abstract T Value { get; set; }
    }

    /// <summary>
    /// Wrapper used to alter the attribute of an object
    /// </summary>
    public abstract class SomeSetBinding<T> : AnyBinding
    {
        public abstract T Value { set; }
    }
}
