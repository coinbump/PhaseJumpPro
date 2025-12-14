using System.Collections;
using System.Collections.Generic;
using Godot;

/*
 * RATING: 5 stars
 * Simple animation utility
 * GODOT CODE REVIEW: 12/30/23
 */
namespace PJ
{
    /// <summary>
    /// When it receives or loses focus, will scale itself accordingly
    /// </summary>
    public partial class ScaleEffect : SomeAnimatedEffect
    {
        [Export]
        public float offScale = 1.0f;

        [Export]
        public float onScale = 2.0f;

        public override void UpdateEffectProperties()
        {
            base.UpdateEffectProperties();

            var valveState = valve.ValveState;
            var transformScale = offScale + (onScale - offScale) * valveState;

            var target = Target;
            if (target is Node2D node2D)
            {
                node2D.Scale = new(transformScale, transformScale);
            }
            else if (target is Node3D node3D)
            {
                node3D.Scale = new(transformScale, transformScale, transformScale);
            }
        }
    }
}
