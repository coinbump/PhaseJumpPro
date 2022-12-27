using System;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/26/22
*/
namespace PJ
{
    /// <summary>
    /// Modifies the object's main render color
    /// </summary>
    public class ColorEffect : SomeAnimatedEffect
    {
        public Color32 offColor = Color.white;
        public Color32 onColor = Color.white;

        public override void UpdateEffectProperties()
        {
            base.UpdateEffectProperties();

            // Bugfix: Changing the color while in the editor causes prefabs to lose their material
            if (Application.isEditor && !Application.isPlaying) { return; }

            var valveState = valve.ValveState;
            MultiRenderer.Color = Color32.Lerp(offColor, onColor, valveState);
        }
    }
}
