using System.Collections;
using System.Collections.Generic;
using PJ;
using Godot;

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
    public abstract partial class SomeEffect : SwitchHandler
    {
        protected MeshMaterialType meshMaterialType = MeshMaterialType.Copy;
        private RendererTool rendererTool;

        public Node Target => GetParent();

        protected RendererTool RendererTool
        {
            get
            {
                if (null == rendererTool)
                {
                    rendererTool = new(Target);
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
