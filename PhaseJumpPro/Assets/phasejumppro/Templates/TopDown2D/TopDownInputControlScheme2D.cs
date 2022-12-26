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

        protected GoNode2D node;

        public SomeValueTransform<float> interpolate = new LinearInterpolate();

        protected override void Start()
        {
            node = GetComponent<GoNode2D>();
            if (null == node)
            {
                Debug.LogWarning("Axis Control Scheme requires a Node2D");
                return;
            }
        }

        protected override void Update()
        {
        }

        public void OnInputMove(Vector2 value)
        {
            if (null == node) { return; }

            // Don't use Node2D Velocity (it is constant). Use rigidbody velocity, which decays
            // This results in more fluid movement
            node.VelocityType = GoNode2D.MoveType.None;

            //Debug.Log("Input Move: " + context.ToString());

            var angleAxisLimiter = new AngleAxisLimiter2D(axisLimit);
            var rigidbody = node.GetComponent<Rigidbody2D>();

            var axisVector = value;
            var angle = new Angle(axisVector);
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
            var velocity = limitedAngle.ToVector2(moveSpeed);

            rigidbody.velocity = velocity;
        }
    }
}
