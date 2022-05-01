﻿using System;
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
        Enter, Stay, Exit
    }

    /// <summary>
    /// Determines what should be sensed
    /// </summary>
    public interface SensorDelegate
    {
        public bool IsSenseTarget(GameObject targetObject);
    }

    /// <summary>
    /// Listens to sense events
    /// </summary>
    public interface SomeSensorListener
    {
        public void OnSense(List<GameObject> objectList, SenseState senseState);
    }

    /// <summary>
    /// A sensor works in conjunction with colliders and sends sense events
    /// Use these instead of simple colliders, and eventually sensors will add collusion checks for walls
    /// </summary>
    public abstract class SomeSensor : MonoBehaviour
    {
        public WeakReference<SensorDelegate> sensorDelegate;

        protected HashSet<HashedWeakReference<SomeSensorListener>> sensorListeners = new HashSet<HashedWeakReference<SomeSensorListener>>();

        public void AddListener(SomeSensorListener listener)
        {
            sensorListeners.Add(new HashedWeakReference<SomeSensorListener>(listener));
        }

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
            ForwardSense(new List<GameObject>() { collider.gameObject }, CollisionState.Exit);
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
            var newListeners = new HashSet<HashedWeakReference<SomeSensorListener>>();

            foreach (HashedWeakReference<SomeSensorListener> listener in sensorListeners)
            {
                if (listener.Reference.TryGetTarget(out SomeSensorListener target))
                {
                    // Keep listeners that were not disposed
                    newListeners.Add(listener);

                    switch (collisionState)
                    {
                        case CollisionState.Enter:
                            target.OnSense(objectList, SenseState.Enter);
                            break;
                        case CollisionState.Stay:
                            target.OnSense(objectList, SenseState.Stay);
                            break;
                        case CollisionState.Exit:
                            target.OnSense(objectList, SenseState.Exit);
                            break;
                    }
                }
            }

            // Clean up any old weak references with null targets
            sensorListeners = newListeners;
        }

        protected abstract void OnSense(GameObject target, CollisionState collisionState);
    }
}
