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
        public Vector3 offset = Vector3.zero;

        public bool orientToPath = true;
        public float orientDegreeAngle = -90.0f;

        [Tooltip("Optional normalized positions (0-1.0), otherwise default will be used.")]
        public List<float> positions = new List<float>();

        protected override void Awake()
        {
            base.Awake();
        }

        public override void ApplyLayout()
        {
            var path = NewPath();

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

                //Debug.Log("Apply layout Position: " + position.ToString() + " Normal: " + normalPosition.ToString());
                childObject.transform.localPosition = position;

                if (orientToPath)
                {
                    var prevPosition = normalPosition;
                    var nextPosition = normalPosition;
                    var orientDelta = .001f;
                    if (normalPosition == 0)
                    {
                        nextPosition += orientDelta;
                    }
                    else if (normalPosition == 1.0f)
                    {
                        prevPosition -= orientDelta;
                    }
                    else
                    {
                        nextPosition = Mathf.Min(1.0f, nextPosition + orientDelta);
                    }

                    var rotationDegreeAngle = AngleUtils.Vector2ToDegreeAngle(path.PositionAt(nextPosition) - path.PositionAt(prevPosition));
                    rotationDegreeAngle += orientDegreeAngle;
                    childObject.transform.localEulerAngles = new Vector3(0, 0, -rotationDegreeAngle);
                }
                else
                {
                    childObject.transform.localEulerAngles = new Vector3(0, 0, 0);
                }

                index++;
            }
        }

        protected abstract SomePath NewPath();
    }
}
