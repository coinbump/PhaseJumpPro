using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/27/22
*/
namespace PJ
{
    /// <summary>
    /// Applies an effect when the pointer is over the object
    /// </summary>
    public class HoverHandler : SomeHoverGestureHandler
    {
        public SomeEffect hoverEffect;

        [Tooltip("Additional Effects")]
        public List<SomeEffect> moreHoverEffects = new();

        List<SomeEffect> HoverEffects
        {
            get
            {
                List<SomeEffect> result = new(moreHoverEffects);
                if (hoverEffect)
                {
                    result.Add(hoverEffect);
                }
                return result;
            }
        }

        public override bool IsHovering
        {
            set
            {
                foreach (var hoverEffect in HoverEffects)
                {
                    hoverEffect.IsOn = value;
                }
            }
        }
    }
}
