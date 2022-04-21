using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple interface
 * CODE REVIEW: 4/14/22
 */
namespace PJ
{
    public abstract class SomeTransformLimiter : MonoBehaviour
    {
        public enum ScanType
        {
            // Objects are responsible for asking this object to limit them (via TransformLimiterTarget)
            FromTarget,

            // Scan child objects only
            Children
        }

        public enum TargetLimitType
        {
            // Limit by object center
            Center,

            // Limit by object bounds
            Bounds

            // FUTURE: Use target collider bounds to determine limits
            //Collider
        }

        // FUTURE: support keep inside/outside if needed
        //public enum LimiterType
        //{
        //    KeepInside, KeepOutside
        //}

        public ScanType scanType = ScanType.FromTarget;
        public TargetLimitType targetLimitType = TargetLimitType.Center;

        public abstract void Scan(GameObject target);

        public void LateUpdate()
        {
            switch (scanType)
            {
                case ScanType.Children:
                    foreach (Transform childTransform in gameObject.transform)
                    {
                        Scan(childTransform.gameObject);
                    }
                    break;
                default:
                    break;
            }
        }

        protected void UpdatePosition(GameObject target, Vector3 newPosition)
        {
            if (target.transform.position != newPosition)
            {
                //Debug.Log("Some Transform limited");

                target.transform.position = newPosition;
                OnPositionUpdated(target);
            }
        }

        protected virtual void OnPositionUpdated(GameObject target) { }
    }
}
