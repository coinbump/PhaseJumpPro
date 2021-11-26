using UnityEngine;
using System.Collections;
using System;

namespace PJ
{
    /// <summary>
    /// 2D x-y axis control scheme for Kinematic objects
    /// </summary>
    public class KAxisControlScheme2D : SomeInputControlScheme
    {
        public Vector2 minMoveSpeed = Vector2.zero;
        public Vector2 maxMoveSpeed = new Vector2(1.0f, 1.0f);
        public Vector2 deadZone = Vector2.zero;

        // Non-zero values indicate interpolation
        public Vector2 interpolate = Vector2.zero;

        public void Update()
        {
            var xAxis = Input.GetAxis("Horizontal");
            var yAxis = Input.GetAxis("Vertical");

            var xMove = moveValue(deadZone.x, xAxis, minMoveSpeed.x, maxMoveSpeed.x, interpolate.x != 0);
            var yMove = moveValue(deadZone.y, yAxis, minMoveSpeed.y, maxMoveSpeed.y, interpolate.y != 0);

            var node = GetComponent<GoNode2D>();
            if (null == node)
            {
                Debug.LogWarning("Axis Control Scheme requires a GoNode2D");
                return;
            }

            node.velocity = new Vector2(xMove, yMove);
        }

        private float moveValue(float deadZone, float axis, float minMoveSpeed, float maxMoveSpeed, bool isInterpolated)
        {
            if (Math.Abs(axis) <= deadZone)
            {
                return 0;
            }

            var result = maxMoveSpeed;

            if (isInterpolated)
            {
                var interpolate = new InterpolateLinear();
                var factor = (Math.Abs(axis) - deadZone) / (1.0f - deadZone);
                var interpolatedFactor = interpolate.Evaluate(factor);
                result = minMoveSpeed + ((maxMoveSpeed - minMoveSpeed) * interpolatedFactor);
            }

            return axis < 0 ? -result : result;
        }
    }
}
