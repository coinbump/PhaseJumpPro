using System;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/25/22
*/
namespace PJ
{
    /// <summary>
    /// Modifies the object's main render color
    /// </summary>
    public class ModifyColorEffect : AnimatedEffect
    {
        public Color32 offColor;
        public Color32 onColor;

        protected MultiRenderer multiRenderer;

        MultiRenderer ThisMultiRenderer
        {
            get
            {
                if (null == multiRenderer)
                {
                    multiRenderer = new(gameObject);
                    multiRenderer.meshMaterialType = MeshMaterialType.Copy;
                }
                return multiRenderer;
            }
        }

        protected override void UpdateAnimatableProperties()
        {
            base.UpdateAnimatableProperties();

            // Bugfix: Changing the color while in the editor causes prefabs to lose their material
            if (Application.isEditor && !Application.isPlaying) { return; }

            var valveState = valve.ValveState;
            ThisMultiRenderer.Color = Color32.Lerp(offColor, onColor, valveState);
        }
    }
}
