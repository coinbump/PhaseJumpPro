using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple utility class for 2D games with Unit Tests
 * CODE REVIEW: 11/23/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Core 2D node code
    /// </summary>
    public class NodeCore2D : Node2D
    {
        public virtual void OnTransformLimited() { }
    }
}