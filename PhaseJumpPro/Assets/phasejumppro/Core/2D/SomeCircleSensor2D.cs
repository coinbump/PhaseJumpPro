using System;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// A 2D sensor that uses a circle collider
    /// </summary>
    public abstract class SomeCircleSensor2D : SomeSensor
    {
        /// <summary>
        /// When a target is sensed, this is the step for checking if the target
        /// is visible within the sensor range
        /// </summary>
        public float occluderCheckAngleStep = 5.0f;

        /// <summary>
        /// The list of type tags that mark occluders (Example: "wall")
        /// </summary>
        public List<string> occluderTypes = new List<string>();

        protected CircleCollider2D circleCollider;

        /// <summary>
        /// Setup the circle collider
        /// </summary>
        protected override void Awake()
        {
            base.Awake();

            circleCollider = GetComponent<CircleCollider2D>();
            if (null == circleCollider)
            {
                circleCollider = gameObject.AddComponent<CircleCollider2D>();
            }

            circleCollider.isTrigger = true;
        }

        /// <summary>
        /// Check if there are occluders in front of the target, if so, return true
        /// </summary>
        protected virtual bool CheckOccluders(float minDegreeAngle, float maxDegreeAngle, GameObject target)
        {
            if (occluderTypes.Count <= 0) { return false; }

            var distance = AngleUtils.Distance(transform.position, target.transform.position);

            var canSeeTarget = false;
            for (float i = minDegreeAngle; i <= maxDegreeAngle; i += occluderCheckAngleStep)
            {
                canSeeTarget = CanSeeTarget(AngleUtils.DegreeAngleToVector2(i, circleCollider.radius), distance, target);
                if (canSeeTarget)
                {
                    break;
                }
            }

            if (!canSeeTarget)
            {
                //Debug.Log("Target " + target.name + " is hidden behind occluder");
                ForwardSense(new List<GameObject>() { target }, CollisionState.Exit);
                return true;
            }

            return false;
        }

        protected virtual bool CanSeeTarget(Vector2 deltaVector, float distanceToTarget, GameObject target)
        {
            var raycastHits = Physics2D.RaycastAll(transform.position, deltaVector, circleCollider.radius);
            HashSet<GameObject> hitObjects = new HashSet<GameObject>();

            foreach (RaycastHit2D raycastHit in raycastHits)
            {
                var hitObject = raycastHit.transform.gameObject;
                hitObjects.Add(hitObject);
            }

            // No point in checking if the sense target isn't hit by the ray.
            if (!hitObjects.Contains(target))
            {
                return false;
            }

            foreach (RaycastHit2D raycastHit in raycastHits)
            {
                //Debug.Log("Sensor RaycastHit: " + raycastHit.transform.gameObject.name);

                var hitObject = raycastHit.transform.gameObject;
                if (!IsOccluder(hitObject)) { continue; }

                var hitDistance = AngleUtils.Distance(transform.position, hitObject.transform.position);

                // We have an occluder and it is nearer than the sensed object
                if (hitDistance < distanceToTarget)
                {
                    //Debug.Log("Sensor Occluder: " + raycastHit.transform.gameObject.name);

                    // False: there is something occluding
                    return false;
                }
            }

            return true;
        }

        protected virtual bool IsOccluder(GameObject hitObject)
        {
            var node = hitObject.GetComponent<SomeNode>();
            if (null == node) { return false; }

            foreach (string type in occluderTypes)
            {
                if (node.HasTypeTag(type))
                {
                    return true;
                }
            }
            return false;
        }
    }
}
