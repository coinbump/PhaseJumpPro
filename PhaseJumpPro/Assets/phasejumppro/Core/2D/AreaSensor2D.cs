using System;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 4 stars
 * Simple sensor. Missing occlusion checks
 * CODE REVIEW: 4/26/22
 */
// FUTURE: add occlusion checks for walls
namespace PJ
{
    /// <summary>
    /// Senses in a circle range around the node
    /// </summary>
    // FUTURE: support occlusion checks for walls
    [RequireComponent(typeof(CircleCollider2D))]
    public class AreaSensor2D : SomeSensor
    {
        protected override void Awake()
        {
            base.Update();

            var circleCollider = GetComponent<CircleCollider2D>();
            if (null == circleCollider)
            {
                circleCollider = gameObject.AddComponent<CircleCollider2D>();
            }

            circleCollider.isTrigger = true;
        }

        protected virtual void OnValidate()
        {
        }

        protected override void OnSense(GameObject target, CollisionState collisionState)
        {
            if (!this.sensorDelegate.TryGetTarget(out SensorDelegate sensorDelegate)) { return; }
            if (!sensorDelegate.IsSenseTarget(target)) { return; }

            List<GameObject> objectList = new List<GameObject>();
            objectList.Add(target);

            ForwardSense(objectList, collisionState);
        }
    }
}
