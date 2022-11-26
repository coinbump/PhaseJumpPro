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
    public class RendererAlphaBinding : SomeBinding<float>
    {
        public WeakReference<GameObject> target;

        protected MultiRenderer multiRenderer;

        public override float Value
        {
            get => multiRenderer.Color.a;
            set
            {
                Color color = multiRenderer.Color;
                color.a = value;
                multiRenderer.Color = color;
            }
        }

        public RendererAlphaBinding(GameObject target)
        {
            this.target = new WeakReference<GameObject>(target);

            multiRenderer = new MultiRenderer(target);
        }
    }
}
