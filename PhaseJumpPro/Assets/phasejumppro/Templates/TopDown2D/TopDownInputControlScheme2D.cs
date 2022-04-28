using UnityEngine;
using System.Collections;
using System;
using UnityEngine.InputSystem;

/*
 * RATING: 5 stars
 * Tested, works
 * CODE REVIEW: 4/16/22
 */
namespace PJ
{
    /// <summary>
    /// 2D x-y axis control scheme for Kinematic objects
    /// </summary>
    /// FUTURE: add 4-way, 8 way input types if needed
    public class TopDownInputControlScheme2D : SomeInputControlScheme
    {
        // NOTE: Acceleration-based input feels floaty and isn't much fun, use velocity instead

        public float minMoveSpeed;
        public float maxMoveSpeed = 1.0f;
        public float deadZone = 0.025f;

        /// <summary>
        /// Limits axes of movement to N axes
        /// </summary>
        public int axisLimit = 0;

        protected Node2D node;

        public SomeTransform<float> interpolate = new InterpolateLinear();

        public void Start()
        {
            node = GetComponent<Node2D>();
            if (null == node)
            {
                Debug.LogWarning("Axis Control Scheme requires a Node2D");
                return;
            }
        }

        public void Update()
        {
        }

        public void OnInputMove(InputAction.CallbackContext context)
        {
            if (null == node) { return; }

            // Don't use Node2D Velocity (it is constant). Use rigidbody velocity, which decays
            // This results in more fluid movement
            node.VelocityType = Node2D.MoveType.None;

            //Debug.Log("Input Move: " + context.ToString());

            var angleAxisLimiter = new AngleAxisLimiter2D(axisLimit);
            var rigidbody = node.GetComponent<Rigidbody2D>();

            var axisVector = context.ReadValue<Vector2>();
            var angle = AngleUtils.Vector2ToDegreeAngle(axisVector);
            var distance = AngleUtils.Distance(Vector2.zero, axisVector);
            if (distance <= deadZone)
            {
                rigidbody.velocity = Vector2.zero;
                return;
            }

            var factor = (distance - deadZone) / (1.0f - deadZone);
            var interpolatedFactor = interpolate.Transform(factor);
            var moveSpeed = minMoveSpeed + ((maxMoveSpeed - minMoveSpeed) * interpolatedFactor);

            var limitedAngle = angleAxisLimiter.LimitAngle(angle);
            var velocity = AngleUtils.DegreeAngleToVector2(limitedAngle, moveSpeed);

            rigidbody.velocity = velocity;
        }
    }
}
