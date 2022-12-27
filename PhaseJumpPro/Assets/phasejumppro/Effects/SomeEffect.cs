using System.Collections;
using System.Collections.Generic;
using PJ;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple abstract
 * CODE REVIEW: 12/26/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// A effect is applied when activated
    /// </summary>
    public abstract class SomeEffect : SwitchHandler
    {
        public MeshMaterialType meshMaterialType = MeshMaterialType.Copy;

        private MultiRenderer multiRenderer;

        protected MultiRenderer MultiRenderer
        {
            get
            {
                if (null == multiRenderer)
                {
                    multiRenderer = new(gameObject);
                    multiRenderer.meshMaterialType = meshMaterialType;
                }
                return multiRenderer;
            }
        }

        protected override void OnSwitchChange()
        {
            base.OnSwitchChange();

            UpdateEffectProperties();
        }

        public virtual void UpdateEffectProperties()
        {
        }

        protected override void OnValidate()
        {
            base.OnValidate();

            UpdateEffectProperties();
        }
    }
}
