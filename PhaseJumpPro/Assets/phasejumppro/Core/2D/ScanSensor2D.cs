using System;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 4 stars
 * Test and works. Could use collusion checks
 * CODE REVIEW: 4/26/22
 */
// FUTURE: support occlusion checks for walls (Problem: thin walls)
namespace PJ
{
    /// <summary>
    /// Senses in an arc range in front of the node (no occlusion checks)
    /// </summary>
    // NOTE: because a sensor can be independent of an object's rotation, this might be a child or sibling of the responder
    // USAGE: Attach an ArcMeshBuilder to the object for debugging
    [RequireComponent(typeof(CircleCollider2D))]
    public class ScanSensor2D : SomeCircleSensor2D
    {
        public float degreeAngleScan = 60.0f;

        protected override void OnSense(GameObject target, CollisionState collisionState)
        {
            if (!this.sensorDelegate.TryGetTarget(out SensorDelegate sensorDelegate)) { return; }
            if (!sensorDelegate.IsSenseTarget(target)) { return; }

            var deltaVector = target.transform.position - transform.position;
            var angleToTarget = AngleUtils.Vector2ToDegreeAngle(deltaVector);
            var orientAngle = 360.0f -transform.localEulerAngles.z;
            var minOrientAngle = orientAngle - degreeAngleScan / 2.0f;
            var maxOrientAngle = orientAngle + degreeAngleScan / 2.0f;

            //if (collisionState == CollisionState.Enter) {
            //    Debug.Log("Scan angle to Target: " + angleToTarget.ToString() + " orientAngle: " + orientAngle.ToString());
            //}
            List<GameObject> objectList = new List<GameObject>() { target };

            if (angleToTarget >= minOrientAngle && angleToTarget <= maxOrientAngle)
            {
                //Debug.Log("Scan Sense in range");
                if (CheckOccluders(minOrientAngle, maxOrientAngle, target))
                {
                    return;
                }

                ForwardSense(objectList, collisionState);
            }
            // This is inside the collider, but we are not scanning the entire circle, and it is out of
            // the angle range, so send an exit event
            else
            {
                //Debug.Log("Forward Exit Sense " + target.name + " Angle: " + angleToTarget);
                ForwardSense(objectList, CollisionState.Exit);
            }
        }
    }
}
