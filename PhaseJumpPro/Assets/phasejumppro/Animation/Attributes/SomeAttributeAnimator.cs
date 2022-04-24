using PJ;
using System;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple abstract class
 * CODE REVIEW: 4/23/22
 */
namespace PJ
{
    /// <summary>
    /// Type erasure interface for SomeAttributeAnimator
    /// </summary>
    public abstract class AnyAttributeAnimator
    {
        public abstract void ApplyAttributeValueFor(float startValue, float endValue, float position);
    }

    /// <summary>
    /// An attribute animator can be used to animate a specific attribute of an object
    /// Example: animate the position of an object, or its alpha
    public abstract class SomeAttributeAnimator<T> : AnyAttributeAnimator
    {
        public abstract T MinValue { get; }
        public abstract T MaxValue { get; }

        public abstract T AttributeValue { get; set; }

        /// <summary>
        /// Return the value interpolated between min and max at position (0-10)
        /// </summary>
        public abstract T LerpValueAt(float startValue, float endValue, float position);

        public override void ApplyAttributeValueFor(float startValue, float endValue, float position)
        {
            var value = LerpValueAt(startValue, endValue, position);
            AttributeValue = value;
        }
    }
}
