using System;
using UnityEditor;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple flow layout
 * CODE REVIEW: 1/14/22
 */
namespace PJ
{
    /// <summary>
    /// Flow the objects with non-contextual spacing (object size doesn't matter)
    /// </summary>
    public class VFlow : Layout2D
    {
        public float spacing = 0;

        public override Vector3 Size()
        {
            return new Vector3(0, spacing * (transform.childCount - 1), 0);
        }

        public override void ApplyLayout()
        {
            var firstPos = Size().y / 2;
            var position = firstPos;

            foreach (Transform childTransform in gameObject.transform)
            {
                childTransform.localPosition = new Vector3(0, position, 0);

                position -= spacing;
            }
        }

#if UNITY_EDITOR
        [CustomEditor(typeof(VFlow))]
        public new class Editor : SomeGameObjectsLayout.Editor
        {
        }
#endif
    }
}
