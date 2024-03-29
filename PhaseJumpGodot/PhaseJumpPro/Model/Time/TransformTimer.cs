﻿using System.Collections;
using System.Collections.Generic;
using Godot;

/*
 * RATING: 5 stars
 * Simple utility
 * CODE REVIEW: 1/15/23
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// A timer that runs for N duration, but whose progress is determined by an
    /// transform operation (usually an interpolation curve)
    /// </summary>
    public class TransformTimer : PJ.Timer
    {
        public SomeValueTransform<float> transform;

        public TransformTimer(float duration, SomeRunner.RunType type, SomeValueTransform<float> transform)
            : base(duration, type)
        {
            this.transform = transform;
        }

        public override float Progress
        {
            get
            {
                var progress = base.Progress;
                if (null == transform)
                {
                    return progress;
                }

                return transform.Transform(progress);
            }
        }
    }
}
