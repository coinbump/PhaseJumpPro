﻿using System;
using UnityEngine;

namespace PJ
{
    namespace View
    {
        /// <summary>
        /// A view for rendering a 2D shape
        /// </summary>
        public class Shape : View2D
        {
            public Material strokeMaterial;
            public Material fillMaterial;

            public Shape()
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
}
