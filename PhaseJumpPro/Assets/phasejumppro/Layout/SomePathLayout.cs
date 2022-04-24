using PJ;
using System;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/23/22
 */
namespace PJ
{
    /// <summary>
    /// Distribute children along a path
    /// </summary>
    /// FUTURE: support orient to path
    public abstract class SomePathLayout : SomeGameObjectsLayout
    {
        public SomePath path;
        public Vector3 offset = Vector3.zero;

        [Tooltip("Optional normalized positions (0-1.0), otherwise default will be used.")]
        public List<float> positions = new List<float>();

        protected override void Awake()
        {
            base.Awake();

            path = NewPath();
        }

        protected override void OnValidate()
        {
            path = NewPath();

            base.OnValidate();
        }

        public override void ApplyLayout()
        {
            int childCount = transform.childCount;
            float normalOffset = childCount > 1 ? 1.0f / (float)(childCount - 1) : 0;

            var index = 0;
            foreach (Transform childTransform in transform)
            {
                var childObject = childTransform.gameObject;
                var normalPosition = index * normalOffset;

                if (index < positions.Count)
                {
                    normalPosition = positions[index];
                }

                var position = path.PositionAt(normalPosition);
                position += offset;
                childObject.transform.localPosition = position;

                index++;
            }
        }

        protected abstract SomePath NewPath();

#if UNITY_EDITOR
        [CustomEditor(typeof(SomePathLayout))]
        public new class Editor : SomeGameObjectsLayout.Editor
        {
        }
#endif
    }
}
