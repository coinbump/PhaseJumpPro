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
        protected enum CheckOccludersResult
        {
            CanSeeTarget,
            TargetOccluded
        }

        protected enum CanSeeResult
        {
            TargetNotIn,
            CanSeeTarget,
            TargetOccluded
        }

        /// <summary>
        /// When a target is sensed, this is the step for checking if the target
        /// is visible within the sensor range
        /// </summary>
        public float occluderCheckAngleStep = 5.0f;

        /// <summary>
        /// The list of type tags that mark occluders (Example: "wall", "occlude")
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
        /// Perform a sweep check for occluders, moving from the target's center, outward
        /// until the target is no longer hit by the sweep, or until we have reached degreeAngleSweep from center
        /// </summary>
        protected virtual CheckOccludersResult SweepCheckOccluders(GameObject target, float degreeAngleSweep)
        {
            if (occluderTypes.Count <= 0 || degreeAngleSweep == 0) { return CheckOccludersResult.CanSeeTarget; }

            var deltaVector = target.transform.position - transform.position;
            var distance = AngleUtils.Distance(transform.position, target.transform.position);
            var startDegreeAngle = AngleUtils.Vector2ToDegreeAngle(deltaVector);

            var step = degreeAngleSweep < 0 ? -occluderCheckAngleStep : occluderCheckAngleStep;
            int numSteps = Mathf.RoundToInt(Mathf.Ceil(Mathf.Abs(degreeAngleSweep) / occluderCheckAngleStep));
            var angle = startDegreeAngle;

            while (numSteps > 0)
            {
                var canSeeResult = CanSeeTarget(AngleUtils.DegreeAngleToVector2(angle, circleCollider.radius), target);

                switch (canSeeResult)
                {
                    // We have swept past the target, it was not seen
                    case CanSeeResult.TargetNotIn:
                        return CheckOccludersResult.TargetOccluded;
                    case CanSeeResult.CanSeeTarget:
                        return CheckOccludersResult.CanSeeTarget;
                }

                numSteps -= 1;
                angle += step;

                // We are using fractional steps, so we might have passed the final angle
                if (degreeAngleSweep > 0)
                {
                    angle = Mathf.Min(startDegreeAngle + degreeAngleSweep, angle);
                }
                else
                {
                    angle = Mathf.Max(startDegreeAngle + degreeAngleSweep, angle);
                }
            }

            return CheckOccludersResult.TargetOccluded;
        }




        /// <summary>
        /// Check if there are occluders in front of the target, if so, return true
        /// </summary>
        protected virtual bool CheckOccluders(float minDegreeAngle, float maxDegreeAngle, GameObject target)
        {
            // TODO:
            //// Check the final angle
            //if (numSteps > 0)
            //{

            //}

            //if (!canSeeTarget)
            //{
            //    //Debug.Log("Target " + target.name + " is hidden behind occluder");
            //    ForwardSense(new List<GameObject>() { target }, CollisionState.Exit);
            //    return true;
            //}

            return false;
        }

        /// <summary>
        /// Performs a raycast and determines if the target is in the ray, and if so, are there any
        /// occluders nearer in the ray than the target
        /// </summary>
        protected virtual CanSeeResult CanSeeTarget(Vector2 deltaVector, GameObject target)
        {
            var raycastHits = Physics2D.RaycastAll(transform.position, deltaVector, circleCollider.radius);
            HashSet<GameObject> hitObjects = new HashSet<GameObject>();

            var distanceToTarget = 0.0f;

            foreach (RaycastHit2D raycastHit in raycastHits)
            {
                var hitObject = raycastHit.transform.gameObject;
                hitObjects.Add(hitObject);

                if (hitObject == target)
                {
                    distanceToTarget = raycastHit.distance;
                }
            }

            // No point in checking if the sense target isn't hit by the ray
            if (!hitObjects.Contains(target))
            {
                return CanSeeResult.TargetNotIn;
            }

            foreach (RaycastHit2D raycastHit in raycastHits)
            {
                //Debug.Log("Sensor RaycastHit: " + raycastHit.transform.gameObject.name);

                var hitObject = raycastHit.transform.gameObject;
                if (!IsOccluder(hitObject)) { continue; }

                var hitDistance = raycastHit.distance;

                // We have an occluder and it is nearer than the sensed object
                if (hitDistance < distanceToTarget)
                {
                    //Debug.Log("Sensor Occluder: " + raycastHit.transform.gameObject.name);

                    // False: there is something occluding
                    return CanSeeResult.TargetOccluded;
                }
            }

            return CanSeeResult.CanSeeTarget;
        }

        /// <summary>
        /// Returns true if the object's SomeNode component has one of the collider type tags
        /// </summary>
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
