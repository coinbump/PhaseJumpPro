using System.Collections.Generic;
using UnityEngine;
using System;

/*
RATING: 5 stars
Simple base
CODE REVIEW: 12/31/22
*/
namespace PJ
{
    /// <summary>
    /// Provides information on how to spawn emits
    /// </summary>
    public abstract class SomeEmitsBuilder
    {
        public abstract List<Emit> BuildEmits();
    }

    /// <summary>
    /// Emit a single object with velocity 0 at position 0
    /// </summary>
    public class BasicEmitsBuilder : SomeEmitsBuilder
    {
        public override List<Emit> BuildEmits()
        {
            List<Emit> result = new();
            result.Add(new(Vector3.zero, Vector3.zero, 0, 0));
            return result;
        }
    }
}
