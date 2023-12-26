using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 12/26/22
 */
namespace PJ
{
    /// <summary>
    /// Modulates object position (for kinematic objects)
    /// Useful for bouncing arrows, pointing at something to draw your attention
    /// IMPORTANT: this requires that the object be parented
    /// </summary>
    public class AnimateSinOffsetEffect : SomeEffect
    {
        public float cycleTime = 1.0f;
        public float scale = 1.0f;
        public Axis axis = Axis.Y;

        protected float timeValue = 0.0f;

        protected override void Start()
        {
            // We need a parent object as a point of reference for the offset
            if (null == transform.parent)
            {
                Debug.Log("Error. Sin animation requires parent");
            }
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            if (!IsOn) { return; }
            if (null == transform.parent) { return; }

            timeValue += time.delta;

            float offset = Mathf.Sin(timeValue * 2 * Mathf.PI / cycleTime) * scale;

            var localPosition = transform.localPosition;

            switch (axis)
            {
                case Axis.X:
                    transform.localPosition = new Vector3(offset, localPosition.y, localPosition.z);
                    break;
                case Axis.Y:
                    transform.localPosition = new Vector3(localPosition.x, offset, localPosition.z);
                    break;
                case Axis.Z:
                    transform.localPosition = new Vector3(localPosition.x, localPosition.y, offset);
                    break;
            }
        }
    }
}
