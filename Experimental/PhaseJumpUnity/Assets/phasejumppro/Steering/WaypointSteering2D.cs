using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 12/29/22
 */
namespace PJ
{
    /// <summary>
    /// Steering in 2D space to reach a waypoint until we are near enough
    /// </summary>
    // FUTURE: support speed up, slow down for more realistic looking travel
    public class WaypointSteering2D : SomeSteering2D, SomeGoStateListener<WaypointSteering2D.StateType>
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

        public GoCore<StateType> core;

        public WaypointSteering2D()
        {
            this.core = new(this);
        }

        protected override void Update()
        {
            switch (core.State)
            {
                case StateType.Arrived:
                    return;
            }

            var node = GetComponent<GoNode2D>();
            if (null == node) { return; }

            var distanceToTarget = AngleUtils.Distance(new Vector2(transform.position.x, transform.position.y), waypoint);
            if (distanceToTarget <= arriveDistance)
            {
                core.State = StateType.Arrived;
                node.Velocity = Vector2.zero;
                return;
            }

            var degreeAngle = new Angle(new Vector2(waypoint.x - transform.position.x, waypoint.y - transform.position.y));
            node.Velocity = degreeAngle.ToVector2(velocity);

            switch (orientType)
            {
                case OrientType.Rotate:
                    node.Rotation = degreeAngle;
                    break;
            }
        }

        public void OnStateChange(GoStateMachine<StateType> inStateMachine)
        {
            var node = GetComponent<GoNode2D>();
            if (null == node) { return; }

            switch (core.State)
            {
                case StateType.Arrived:
                    node.Velocity = Vector2.zero;
                    break;
                default:
                    break;
            }
        }

        public void OnStateFinish(GoStateMachine<StateType> inStateMachine)
        {
        }
    }
}
