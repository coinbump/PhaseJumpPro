using System;
using UnityEngine;

/*
RATING: 5 stars
Solid foundation
CODE REVIEW: 12/20/22
*/
namespace PJ
{
    /// <summary>
    /// Adds common behavior such as basic velocity and acceleration
    /// </summary>
    [RequireComponent(typeof(Rigidbody2D))]
    public class GoNode2D : NodeCore2D
    {
        /// <summary>
        /// Type for velocity
        /// </summary>
        public enum MoveType
        {
            // No fixed movement (use forces)
            None,

            // Moves in direction it is facing (based on 0 degree meaning up)
            Forward,

            // Constant velocity (re-applied each frame)
            Vector
        }

        [Header("Node2D Properties")]

        protected SpriteRenderer spriteRenderer;

        public Vector2 Velocity
        {
            get => velocity;
            set
            {
                velocityType = MoveType.Vector;
                velocity = value;

                if (null != maxVelocity)
                {
                    velocity = Vector2.ClampMagnitude(velocity, maxVelocity.value);
                }

                UpdateVelocity();
            }
        }

        public float ForwardVelocity
        {
            get => velocity.x;
            set
            {
                velocityType = MoveType.Forward;

                velocity.x = value;
                velocity.y = 0;

                if (null != maxVelocity)
                {
                    velocity = Vector2.ClampMagnitude(velocity, maxVelocity.value);
                }

                UpdateVelocity();
            }
        }

        public Vector2 Acceleration
        {
            get => acceleration;
            set => acceleration = value;
        }

        public float MaxVelocity
        {
            set => maxVelocity = new Optional<float>(value);
        }

        public MoveType VelocityType
        {
            get => velocityType;
            set => velocityType = value;
        }

        [SerializeField]
        protected MoveType velocityType = MoveType.None;

        [SerializeField]
        protected Vector2 velocity = new Vector2(0, 0);

        protected Vector2 acceleration = Vector2.zero;
        protected Optional<float> maxVelocity;

        protected override void Awake()
        {
            base.Awake();

            // Some 2D objects use sprites for rendering
            if (TryGetComponent(out SpriteRenderer spriteRenderer))
            {
                this.spriteRenderer = spriteRenderer;
            }
        }

        protected override void Start()
        {
            base.Start();

            UpdateVelocity();
        }

        protected virtual void UpdateVelocity()
        {
            if (null == rigidbody) { return; }
            if (rigidbody.bodyType == RigidbodyType2D.Static) { return; }

            Vector2 velocityVector = Vector2.zero;
            switch (velocityType)
            {
                case MoveType.None:
                    return;
                case MoveType.Forward:
                    velocityVector = Rotation.ToVector2(velocity.x);
                    break;
                case MoveType.Vector:
                    velocityVector = velocity;
                    break;
            }

            rigidbody.velocity = velocityVector;
        }

        protected override void FixedUpdate()
        {
            base.FixedUpdate();

            CheckAcceleration(new TimeSlice(Time.deltaTime), UpdateType.Fixed);

            // Fixed velocity must be re-applied each frame
            if (ShouldMoveForUpdate(UpdateType.Fixed))
            {
                UpdateVelocity();
            }
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            CheckAcceleration(new TimeSlice(Time.deltaTime), UpdateType.Default);

            // Fixed velocity must be re-applied each frame
            if (ShouldMoveForUpdate(UpdateType.Default))
            {
                UpdateVelocity();
            }
        }

        protected void CheckAcceleration(TimeSlice time, UpdateType updateType)
        {
            // Check velocity
            if (!ShouldMoveForUpdate(updateType)) { return; }

            if (Vector2.zero == acceleration) { return; }

            switch (velocityType)
            {
                case MoveType.Forward:
                    ForwardVelocity = ForwardVelocity + acceleration.x * time.delta;
                    break;
                default:
                    Velocity = velocity + acceleration * time.delta;
                    break;
            }
        }

        protected bool ShouldMoveForUpdate(UpdateType updateType)
        {
            bool isFixedUpdate = updateType == UpdateType.Fixed;
            var isKinematic = IsKinematic;

            // Kinematic objects move in Update
            // Physics objects move in FixedUpdate
            return isKinematic ? !isFixedUpdate : isFixedUpdate;
        }
    }
}
