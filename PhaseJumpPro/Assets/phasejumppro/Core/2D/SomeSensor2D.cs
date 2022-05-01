using System;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 5/1/22
 */
namespace PJ
{
    /// <summary>
    /// A 2D sensor. Occlusions are checked by a left/right sweep and an angle step
    /// </summary>
    public abstract class SomeSensor2D : SomeSensor
    {
        public abstract float MaxOccluderRaycastDistance { get; }

        /// <summary>
        /// When a target is sensed, this is the step for checking if the target
        /// is visible within the sensor range
        /// </summary>
        public float occluderCheckAngleStep = 1.0f;

        /// <summary>
        /// Perform a sweep check for occluders, moving from the target's center, outward
        /// until the target is no longer hit by the sweep, or until we have reached degreeAngleSweep from center
        /// </summary>
        protected virtual CheckOccludersResult SweepCheckOccluders(GameObject target, float startDegreeAngle, float degreeAngleSweep, bool exitIfTargetNotIn)
        {
            if (occluderTypes.Count <= 0 || degreeAngleSweep == 0) { return CheckOccludersResult.CanSeeTarget; }
            if (occluderCheckAngleStep <= 0) { return CheckOccludersResult.CanSeeTarget; }

            var step = degreeAngleSweep < 0 ? -occluderCheckAngleStep : occluderCheckAngleStep;
            int numSteps = Mathf.RoundToInt(Mathf.Ceil(Mathf.Abs(degreeAngleSweep) / occluderCheckAngleStep));
            var angle = startDegreeAngle;

            while (numSteps > 0)
            {
                var canSeeResult = CanSeeTarget(AngleUtils.DegreeAngleToVector2(angle, MaxOccluderRaycastDistance), target);

                switch (canSeeResult)
                {
                    // We have swept past the target, it was not seen
                    case CanSeeResult.TargetNotIn:
                        // Some sensors start from the object's center and can exit early if the target is not in the current sweep
                        // Other sensors check an angle range and need to perform the entire sweep
                        if (exitIfTargetNotIn)
                        {
                            //Debug.Log("Sweep Target Not In: " + angle);
                            return CheckOccludersResult.TargetOccluded;
                        }
                        break;
                    case CanSeeResult.CanSeeTarget:
                        //Debug.Log("Sweep Can See: " + angle);
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
        /// Check if there are occluders in front of the target via a left and right sweep
        /// If the object is occluded, return true
        /// </summary>
        protected virtual bool CheckOccluders(float startDegreeAngle, float maxAngleSweep, GameObject target, bool exitIfTargetNotIn)
        {
            var leftSweepCheck = SweepCheckOccluders(target, startDegreeAngle, -maxAngleSweep, exitIfTargetNotIn);
            if (leftSweepCheck == CheckOccludersResult.CanSeeTarget)
            {
                return false;
            }

            var rightSweepCheck = SweepCheckOccluders(target, startDegreeAngle, maxAngleSweep, exitIfTargetNotIn);
            if (rightSweepCheck == CheckOccludersResult.CanSeeTarget)
            {
                return false;
            }

            //Debug.Log("Check Occluders Exit");

            ForwardSense(new List<GameObject>() { target }, CollisionState.Exit);
            return true;
        }

        /// <summary>
        /// Performs a raycast and determines if the target is in the ray, and if so, are there any
        /// occluders nearer in the ray than the target
        /// </summary>
        protected virtual CanSeeResult CanSeeTarget(Vector2 deltaVector, GameObject target)
        {
            var raycastHits = Physics2D.RaycastAll(transform.position, deltaVector, MaxOccluderRaycastDistance);
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
                if (hitObject == target) { continue; }
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
