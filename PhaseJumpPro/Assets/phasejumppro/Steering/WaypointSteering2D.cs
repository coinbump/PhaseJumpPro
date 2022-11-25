using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/23/22
 */
namespace PJ
{
    /// <summary>
    /// Steering in 2D space to reach a waypoint until we are near enough
    /// </summary>
    // FUTURE: support speed up, slow down for more realistic looking travel
    public class WaypointSteering2D : SomeSteering2D
    {
        public enum StateType
        {
            Steering,

            Arrived
        }

        public enum OrientType
        {
            None,

            // Rotate to face the target
            Rotate
        }

        public float velocity = 1.0f;
        public Vector2 waypoint;

        public OrientType orientType;

        /// <summary>
        /// Distance that we stop the navigation
        /// </summary>
        public float arriveDistance = 0.025f;

        public StateMachine<StateType> stateMachine { get; protected set; } = new StateMachine<StateType>();

        protected override void Update()
        {
            switch (stateMachine.State)
            {
                case StateType.Arrived:
                    return;
            }

            var node = GetComponent<Node2D>();
            if (null == node) { return; }

            var distanceToTarget = AngleUtils.Distance(new Vector2(transform.position.x, transform.position.y), waypoint);
            if (distanceToTarget <= arriveDistance)
            {
                stateMachine.State = StateType.Arrived;
                node.Velocity = Vector2.zero;
                return;
            }

            var degreeAngle = AngleUtils.Vector2ToDegreeAngle(new Vector2(waypoint.x - transform.position.x, waypoint.y - transform.position.y));
            node.Velocity = AngleUtils.DegreeAngleToVector2(degreeAngle, velocity);

            switch (orientType)
            {
                case OrientType.Rotate:
                    node.Rotation = degreeAngle;
                    break;
            }
        }
    }
}
