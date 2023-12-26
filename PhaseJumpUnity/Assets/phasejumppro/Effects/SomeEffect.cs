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
        protected MeshMaterialType meshMaterialType = MeshMaterialType.Copy;
        private RendererTool rendererTool;

        protected RendererTool MultiRenderer
        {
            get
            {
                if (null == rendererTool)
                {
                    rendererTool = new(gameObject);
                    rendererTool.meshMaterialType = meshMaterialType;
                }
                return rendererTool;
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
