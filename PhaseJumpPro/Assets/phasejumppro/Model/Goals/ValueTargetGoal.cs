﻿using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 12/29/22
 */
namespace PJ
{
    /// <summary>
    /// A goal that must reach a value target to be complete
    /// </summary>
    public class ValueTargetGoal : SomeGoal
    {
        protected int valueTarget;
        protected int value;

        public int ValueTarget
        {
            get => valueTarget;
            set
            {
                valueTarget = value;
            }
        }

        public int Value => value;

        public ValueTargetGoal(int valueTarget, int valueState = 0)
        {
            this.valueTarget = valueTarget;
            this.value = valueState;
        }

        public override float Progress => Mathf.Max(0, MathF.Min(1.0f, (float)value / (float)valueTarget));

        public void Increment()
        {
            if (value >= valueTarget) { return; }

            value++;
            if (value >= valueTarget)
            {
                IsComplete = true;
            }
        }
    }
}
