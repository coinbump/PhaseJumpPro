using System;
using UnityEditor;
using UnityEngine;

/*
 * RATING: 4 stars
 * Works, could use support for kinematic objects
 * CODE REVIEW: 4/17/22
 */
// FUTURE: Support kinematic objects if needed
// FUTURE: Support 3D gravity wells
namespace PJ
{
    /// <summary>
    /// Applies a 2D gravity force within its radius (kinematic objects not supported)
    /// </summary>
    public class GravityWell2D : MonoBehaviour
    {
        public enum ForceType
        {
            // Gravity strength decreases with distance (depends on mass)
            InverseSquareMass,

            // Interpolate forces linearly from min to max
            Linear
        }

        public ForceType forceType = ForceType.InverseSquareMass;
        public float radius = 1.0f;

        /// <summary>
        /// If true, pull the mass value from the rigidbody
        /// </summary>
        public bool useRigidbodyValues = false;

        public float gravityMass = 1.0f;
        
        [Tooltip("Min force to be applied")]
        public float minForce = 1.0f;

        [Tooltip("Max force to be applied")]
        public float maxForce = 2.0f;

        protected new Rigidbody2D rigidbody;

        protected virtual void Start()
        {
            var circleCollider = GetComponent<CircleCollider2D>();
            if (null == circleCollider)
            {
                circleCollider = gameObject.AddComponent<CircleCollider2D>();
            }

            rigidbody = GetComponent<Rigidbody2D>();
            if (null != rigidbody && useRigidbodyValues)
            {
                this.gravityMass = rigidbody.mass;
            }

            circleCollider.radius = radius;
            circleCollider.isTrigger = true;
        }

        protected virtual void OnValidate()
        {
            var circleCollider = GetComponent<CircleCollider2D>();
            if (null == circleCollider) { return; }

            circleCollider.radius = radius;
        }

        protected virtual void OnTriggerStay2D(Collider2D collider)
        {
            var colliderObject = collider.gameObject;
            var colliderWorldCenter = colliderObject.transform.position;
            var thisWorldCenter = transform.position;

            var distance = AngleUtils.Distance(colliderWorldCenter, thisWorldCenter);

            //Debug.Log("Trigger Gravity collider: " + colliderWorldCenter.ToString() + " well: " + thisWorldCenter.ToString() + " Distance: " + distance.ToString());

            if (distance > radius) { return; }

            // Avoid divide by zero
            if (distance == 0 || radius == 0) { return; }

            var rigidbody = colliderObject.GetComponent<Rigidbody2D>();
            if (null == rigidbody) { return; }

            var mass1 = rigidbody.mass;
            var mass2 = gravityMass;

            // Linear force
            var force = minForce + (maxForce - minForce) * (distance / radius);

            switch (forceType)
            {
                case ForceType.InverseSquareMass:
                    force = (mass1 * mass2) / (distance * distance);
                    break;
                case ForceType.Linear:
                    break;
            }

            // Clamp force to prevent strange values near center and weak value at edge
            force = Mathf.Max(minForce, MathF.Min(maxForce, force));

            var vectorToWell = new Vector2(thisWorldCenter.x - colliderWorldCenter.x, thisWorldCenter.y - colliderWorldCenter.y);
            //Debug.Log("Vector to well: " + vectorToWell.ToString() + " Force: " + force.ToString());

            vectorToWell.Normalize();
            var forceVector = vectorToWell * force;
            //Debug.Log("Force Vector: " + forceVector.ToString());

            rigidbody.AddForce(forceVector);
        }

#if UNITY_EDITOR
        protected override void RenderGizmos(EditorUtils.RenderState renderState)
        {
            EditorUtils.RenderCircle(transform.position, radius, renderState);
        }
#endif
    }
}
