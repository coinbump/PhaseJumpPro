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
    public class ScanSensor2D : SomeSensor
    {
        public float degreeAngleScan = 60.0f;

        protected override void Awake()
        {
            base.Awake();

            var circleCollider = GetComponent<CircleCollider2D>();
            if (null == circleCollider)
            {
                circleCollider = gameObject.AddComponent<CircleCollider2D>();
            }

            circleCollider.isTrigger = true;
        }

        protected override void OnSense(GameObject target, CollisionState collisionState)
        {
            if (!this.sensorDelegate.TryGetTarget(out SensorDelegate sensorDelegate)) { return; }
            if (!sensorDelegate.IsSenseTarget(target)) { return; }

            var angleToTarget = AngleUtils.Vector2ToDegreeAngle(target.transform.position - transform.position);
            var orientAngle = 360.0f -transform.localEulerAngles.z;
            var minOrientAngle = orientAngle - degreeAngleScan / 2.0f;
            var maxOrientAngle = orientAngle + degreeAngleScan / 2.0f;

            //if (collisionState == CollisionState.Enter) {
            //    Debug.Log("Scan angle to Target: " + angleToTarget.ToString() + " orientAngle: " + orientAngle.ToString());
            //}

            if (angleToTarget >= minOrientAngle && angleToTarget <= maxOrientAngle)
            {
                //Debug.Log("Scan Sense in range");

                List<GameObject> objectList = new List<GameObject>();
                objectList.Add(target);
                ForwardSense(objectList, collisionState);
            }
        }
    }
}
