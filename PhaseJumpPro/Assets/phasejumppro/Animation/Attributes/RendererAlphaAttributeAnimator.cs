using UnityEngine;
using System;

/*
 * RATING: 5 stars
 * Simple class
 * CODE REVIEW: 4/23/22
 */
namespace PJ
{
    /// <summary>
    /// Animates the renderer's alpha color for fade animations
    /// </summary>
    // NOTE: this will not work with opaque materials
    public class RendererAlphaAttributeAnimator : SomeAttributeAnimator<float>
    {
        public WeakReference<GameObject> target;

        protected MultiRenderer multiRenderer;

        public override float AttributeValue
        {
            get => multiRenderer.Color.a;
            set
            {
                Color color = multiRenderer.Color;
                color.a = value;
                multiRenderer.Color = color;
            }
        }

        public RendererAlphaAttributeAnimator(GameObject target)
        {
            this.target = new WeakReference<GameObject>(target);

            multiRenderer = new MultiRenderer(target);
        }

        public override float MinValue { get => 0; }
        public override float MaxValue { get => 1.0f; }

        public override float LerpValueAt(float startValue, float endValue, float position)
        {
            return startValue + (endValue - startValue) * position;
        }
    }
}
