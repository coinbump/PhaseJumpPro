using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Simple base class
 * CODE REVIEW: 12/26/22
 */
namespace PJ
{
    /// <summary>
    /// For animations that affect an object
    /// </summary>
    public abstract class SomeAnimation : WorldComponent
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
    }
}
