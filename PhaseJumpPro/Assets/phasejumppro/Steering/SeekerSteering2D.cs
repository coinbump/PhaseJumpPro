using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/13/22
 */
namespace PJ
{
    /// <summary>
    /// Use for homing missiles, a seeker seeks its target for N seconds at N turn speed
    /// </summary>
    public class SeekerSteering2D : SomeSteering2D
    {
        public GameObject target;

        /// <summary>
        /// Seek for N seconds then give up
        /// </summary>
        public float seekTime = 0;  // 0 indicates unlimited seek

        /// <summary>
        /// How fast it can turn to target (angles per second)
        /// </summary>
        public float turnSpeedDegrees = 360.0f;

        [SerializeField]
        protected float velocity = 1.0f;

        protected Node2D node;
        protected float seekTimer = 0;

        public bool IsSeeking
        {
            get
            {
                return (seekTime <= 0 || (seekTime > 0 && seekTimer < seekTime));
            }
        }

        public float Velocity
        {
            get => velocity;
            set
            {
                velocity = value;
                if (null != node)
                {
                    node.ForwardVelocity = velocity;
                }
            }
        }

        protected override void Start()
        {
            base.Start();

            node = GetComponent<Node2D>();
            if (null == node)
            {
                Debug.Log("Error. Seeker2D requires a Node2D for ForwardVelocity");
                return;
            }

            node.ForwardVelocity = velocity;
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            if (!IsSeeking) { return; }
            if (null == target) {
                Debug.Log("Error. Seeker needs a target");
                return;
            }

            seekTimer += time.delta;
            if (!IsSeeking)
            {
                OnDoneSeeking();
            }

            var seekerPosition = transform.position;
            var targetPosition = target.transform.position;
            var seekerAngle = node.Rotation;
            var toTargetAngle = AngleUtils.Vector2ToDegreeAngle(targetPosition - seekerPosition);

            float leftTurnAngle;
            float rightTurnAngle;
            if (seekerAngle < toTargetAngle)
            {
                rightTurnAngle = toTargetAngle - seekerAngle;
                leftTurnAngle = 360.0f - rightTurnAngle;
            }
            else
            {
                leftTurnAngle = seekerAngle - toTargetAngle;
                rightTurnAngle = 360.0f - leftTurnAngle;
            }

            //Debug.Log("Seeker Angle: " + seekerAngle.ToString() + "Target Angle: " + toTargetAngle.ToString() + "LeftTurn Angle: " + leftTurnAngle.ToString() + "RightTurn Angle: " + rightTurnAngle.ToString());

            float newSeekerAngle = seekerAngle;

            // Use min/max to avoid wobble with fast turns
            if (leftTurnAngle < rightTurnAngle)
            {
                newSeekerAngle = Mathf.Max(toTargetAngle, seekerAngle - Time.deltaTime * turnSpeedDegrees);
            }
            else
            {
                newSeekerAngle = Mathf.Min(toTargetAngle, seekerAngle + Time.deltaTime * turnSpeedDegrees);
            }

            node.Rotation = newSeekerAngle;
        }

        /// <summary>
        /// Called when the seeker gives up seeking
        /// </summary>
        public virtual void OnDoneSeeking()
        {
            //Debug.Log("Seeker: Done Seeking");
        }
    }
}
