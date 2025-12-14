using System.Collections;
using System.Collections.Generic;
using PJ;
using Godot;

/*
 * RATING: 4 stars
 * Could use unit tests
 * CODE REVIEW: 12/26/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Uses a valve to perform animated changes
    /// </summary>
    public abstract partial class SomeAnimatedEffect : SomeEffect
    {
        [Export]
        protected float turnOnDuration = 0.3f;
        [Export]
        protected float turnOffDuration = 0.3f;

        public float TurnOnDuration
        {
            get => turnOnDuration;
            set
            {
                turnOnDuration = value;
                valve.turnOnTimer.duration = value;
            }
        }

        public float TurnOffDuration
        {
            get => turnOffDuration;
            set
            {
                turnOffDuration = value;
                valve.turnOffTimer.duration = value;
            }
        }

        /// <summary>
        /// Used to animate changes in focus over time
        /// </summary>
        protected Valve valve = new Valve();

        public SomeAnimatedEffect()
        {
            valve.turnOffTimer = new TransformTimer(turnOffDuration, SomeRunner.RunType.RunOnce, new EaseOutSquared());
            valve.turnOnTimer = new TransformTimer(turnOnDuration, SomeRunner.RunType.RunOnce, new EaseOutSquared());
        }

        protected override void Awake()
        {
            base.Awake();

            valve.turnOffTimer.duration = turnOffDuration;
            valve.turnOnTimer.duration = turnOnDuration;
        }

        public override void Start()
        {
            base.Start();

            valve.InputBinary(isOn, InputEffect.Immediate);
            UpdateEffectProperties();
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            float valveState = valve.ValveState;
            valve.OnUpdate(time);
            float newValveState = valve.ValveState;

            // Prevent update thrashing
            if (valveState != newValveState)
            {
                UpdateEffectProperties();
            }
        }

        protected override void OnSwitchChange()
        {
            valve.InputBinary(isOn, InputEffect.Timed);
        }
    }
}
