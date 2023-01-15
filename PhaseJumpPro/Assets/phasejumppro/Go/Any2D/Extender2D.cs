using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using UnityEditor;

/*
 * RATING: 5 stars
 * Useful 2D pattern. Tested and works
 * CODE REVIEW: 12/18/22
 */
namespace PJ
{
    /// <summary>
    /// Extends child objects away from the center of the parent
    /// Useful for animations where the children extend away and collapse in
    /// Example: a selection frame where the corner pieces move away and towards the selection
    /// </summary>
    public class Extender2D : WorldComponent
    {
        [Serializable]
        public struct Item
        {
            /// <summary>
            /// Angle at which the distance-extension occurs in 2D space (in degrees)
            /// </summary>
            [Range(0, 360.0f)]
            public float extendAngle;

            /// <summary>
            /// Min distance from center of parent 
            /// </summary>
            public float minDistance;

            /// <summary>
            /// Max distance from center of parent
            /// </summary>
            public float maxDistance;

            /// <summary>
            /// Normalized distance (0-1.0)
            /// </summary>
            [Range(0, 1.0f)]
            public float normalDistance;

            public float Distance
            {
                get
                {
                    return minDistance + normalDistance * (maxDistance - minDistance);
                }
            }
        }

        /// <summary>
        /// Links all child objects to share the same distance values
        /// </summary>
        public bool linkDistances = true;

        public float minDistance;
        public float maxDistance;

        /// <summary>
        /// If normal distances are linked, parent normal distance will be used for all child objects
        /// Otherwise this is ignored
        /// </summary>
        public float normalDistance = 0;

        public List<Item> items = new();

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            ApplyLayout();
        }

        public virtual Vector2 LocalPositionForElement(Item element)
        {
            var distance = element.Distance;
            if (linkDistances)
            {
                distance = minDistance + normalDistance * (maxDistance - minDistance);
            }

            var offsetVector = Angle.DegreesAngle(element.extendAngle).ToVector2(distance);
            return offsetVector;
        }

        public virtual void ApplyLayout()
        {
            var index = 0;
            foreach (Transform childTransform in transform)
            {
                if (index >= items.Count) { break; }
                var childObject = childTransform.gameObject;

                var element = items[index];
                var position = LocalPositionForElement(element);
                childObject.transform.localPosition = new Vector3(position.x, position.y, childObject.transform.localPosition.z);

                index++;
            }
        }

#if UNITY_EDITOR
        protected virtual void OnValidate()
        {
            ApplyLayout();
        }

        protected override void RenderGizmos(EditorUtils.RenderState renderState)
        {
            foreach (Item element in items)
            {
                var position = transform.TransformPoint(LocalPositionForElement(element));
                EditorUtils.DrawRect(position, 0.05f, 0.05f, renderState);
            }
        }

        [CustomEditor(typeof(Extender2D))]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                if (GUILayout.Button("Apply Layout"))
                {
                    Extender2D _target = (Extender2D)target;
                    _target.ApplyLayout();
                }
            }
        }
#endif
    }
}
