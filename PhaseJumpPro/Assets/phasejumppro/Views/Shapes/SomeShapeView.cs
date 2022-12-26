using System;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// A view for rendering a 2D shape
    /// </summary>
    public abstract class SomeShapeView : View2D
    {
        public Material strokeMaterial;
        public Material fillMaterial;

        public SomeShapeView()
        {
        }

        protected override void OnFrameChange()
        {
            base.OnFrameChange();

            foreach (Transform childTransform in transform) {
                if (childTransform.TryGetComponent(out SomeMeshBuilder meshBuilder)) {
                    meshBuilder.WorldSize = Frame.size;
                    meshBuilder.Build();
                }
            }
        }

        protected void ClearParts()
        {
            foreach (Transform childTransform in transform)
            {
                if (childTransform.TryGetComponent(out SomeMeshBuilder meshBuilder)) {
                    Destroy(childTransform.gameObject);
                }
            }
        }

        protected override void Awake()
        {
            base.Awake();

            ClearParts();
            BuildShape();
        }

        protected abstract void BuildShape();

        /// <summary>
        /// Shapes are composed of multiple parts (frame, fill, etc.)
        /// Build a part and add it as a child object
        /// </summary>
        public GameObject AddPartObject<MeshBuilder>(Material material, float zIndex = 0) where MeshBuilder : SomeMeshBuilder
        {
            if (!material) { return null; }

            var partObject = new GameObject();
            partObject.transform.parent = gameObject.transform;

            var meshRenderer = partObject.AddComponent<MeshRenderer>();
            meshRenderer.material = material;

            var meshBuilder = partObject.AddComponent<MeshBuilder>();

            meshBuilder.WorldSize = Frame.size;
            meshBuilder.Build();

            // Local z must be < 0 to appear in front of parent.
            partObject.transform.localPosition = new Vector3(0, 0, Mathf.Min(-zStep, -zStep * (zIndex + 1)));

            return partObject;
        }
    }
}
