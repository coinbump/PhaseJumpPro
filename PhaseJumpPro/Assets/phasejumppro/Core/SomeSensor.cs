using System;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple abstract class
 * CODE REVIEW: 4/25/22
 */
namespace PJ
{
    public enum CollisionType
    {
        Trigger,

        Collision
    }

    public enum CollisionState
    {
        Enter, Stay, Exit
    }

    public enum SenseState
    {
        Enter, Stay
    }

    /// <summary>
    /// Response to SomeSensor events
    /// </summary>
    public interface SensorDelegate
    {
        public bool IsSenseTarget(GameObject targetObject);
        public void OnSense(List<GameObject> objectList, SenseState senseState);
    }

    /// <summary>
    /// A sensor works in conjunction with colliders and sends sense events
    /// Use these instead of simple colliders, and eventually sensors will add collusion checks for walls
    /// </summary>
    public abstract class SomeSensor : MonoBehaviour
    {
        public WeakReference<SensorDelegate> sensorDelegate;

        protected virtual void OnTriggerEnter2D(Collider2D collider)
        {
            OnSense(collider.gameObject, CollisionState.Enter);
        }

        protected virtual void OnTriggerStay2D(Collider2D collider)
        {
            OnSense(collider.gameObject, CollisionState.Stay);
        }

        protected virtual void OnTriggerExit2D(Collider2D collider)
        {
            OnSense(collider.gameObject, CollisionState.Exit);
        }

        protected virtual void OnCollisionEnter2D(Collision2D collision)
        {
            Debug.Log("Error. Sensor colliders should be triggers");
        }

        protected virtual void Awake()
        {
        }

        protected virtual void Update()
        {
        }

        protected virtual void ForwardSense(List<GameObject> objectList, CollisionState collisionState)
        {
            if (!this.sensorDelegate.TryGetTarget(out SensorDelegate sensorDelegate)) { return; }

            // We pass enter, stay events, but exit is not sensed
            switch (collisionState)
            {
                case CollisionState.Enter:
                    sensorDelegate.OnSense(objectList, SenseState.Enter);
                    break;
                case CollisionState.Stay:
                    sensorDelegate.OnSense(objectList, SenseState.Stay);
                    break;
            }
        }

        protected abstract void OnSense(GameObject target, CollisionState collisionState);
    }
}
