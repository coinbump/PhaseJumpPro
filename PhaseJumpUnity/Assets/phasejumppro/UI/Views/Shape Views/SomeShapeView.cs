using System;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 6/17/23
*/
namespace PJ
{
    /// <summary>
    /// A view for rendering a 2D shape
    /// </summary>
    public abstract class SomeShapeView : View2D
    {
        public Material fillMaterial;
        public Material strokeMaterial;

        public SomeShapeView()
        {
        }

        protected override void OnFrameChange()
        {
            base.OnFrameChange();

            foreach (Transform childTransform in transform)
            {
                if (childTransform.TryGetComponent(out SomeMeshBuilder meshBuilder))
                {
                    meshBuilder.WorldSize2D = Frame.size;
                }
            }
        }

        protected void DestroyParts()
        {
            foreach (Transform childTransform in transform)
            {
                ContextDestroy(childTransform.gameObject);
            }
        }

        protected override void Awake()
        {
            base.Awake();

            RebuildShape();
        }

        public void RebuildShape()
        {
            DestroyParts();
            BuildShape();
        }

        protected abstract void BuildShape();

        /// <summary>
        /// Shapes are composed of multiple parts (frame, fill, etc.)
        /// Build a part and add it as a child object
        /// </summary>
        public GameObject AddPartObject<MeshBuilder>(String id, Material material, int zIndex) where MeshBuilder : SomeMeshBuilder
        {
            if (!material) { return null; }

            GameObject partObject;
            var existingPartTransform = transform.Find(id);
            if (null != existingPartTransform)
            {
                Debug.Log("WARNING. Tried to add the same part twice: " + id);
                partObject = existingPartTransform.gameObject;
            }
            else
            {
                partObject = new GameObject();
                partObject.name = id;
                partObject.transform.parent = gameObject.transform;

                var meshRenderer = partObject.AddComponent<MeshRenderer>();
                meshRenderer.material = material;

                var meshBuilder = partObject.AddComponent<MeshBuilder>();

                meshBuilder.WorldSize2D = Frame.size;
            }

            // Local z must be < 0 to appear in front of parent.
            partObject.transform.localPosition = new Vector3(0, 0, Mathf.Min(-zStep, -zStep * (zIndex + 1)));

            return partObject;
        }

#if UNITY_EDITOR
        protected override void OnValidate()
        {
            base.OnValidate();
        }

        [CustomEditor(typeof(SomeShapeView), true)]
        public new class Editor : View2D.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                SomeShapeView theTarget = (SomeShapeView)target;
                if (GUILayout.Button("Build Shape"))
                {
                    theTarget.RebuildShape();
                }
            }
        }
#endif
    }
}
