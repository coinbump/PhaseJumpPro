using System.Collections;
using System.Collections.Generic;
using PJ;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple animation utility
 * CODE REVIEW: 11/13/22
 */
namespace PJ
{
    /// <summary>
    /// Uses a valve to perform animated changes when focus changes
    /// When this object receives focus, the valve is opened over N time
    /// Check the valve's state to find a normalized value (0-1.0) to use for animations
    /// </summary>
    public abstract class AnimatedEffect : SomeEffect
    {
        public float turnOnTime = 0.3f;
        public float turnOffTime = 0.3f;

        public float TurnOnTime
        {
            get => turnOnTime;
            set
            {
                turnOnTime = value;

                if (null == valve.turnOnTimer) { return; }
                valve.turnOnTimer.duration = value;
            }
        }

        public float TurnOffTime
        {
            get => turnOffTime;
            set
            {
                turnOffTime = value;

                if (null == valve.turnOffTimer) { return; }
                valve.turnOffTimer.duration = value;
            }
        }

        /// <summary>
        /// Used to animate changes in focus over time
        /// </summary>
        protected Valve valve = new Valve();

        protected override void Awake()
        {
            base.Awake();

            valve.turnOffTimer = new TransformTimer(turnOffTime, SomeRunner.RunType.RunOnce, new EaseOutSquared());
            valve.turnOnTimer = new TransformTimer(turnOnTime, SomeRunner.RunType.RunOnce, new EaseOutSquared());
        }

        protected override void Start()
        {
            base.Start();

            valve.InputBinary(isOn, InputEffect.Immediate);
            UpdateAnimatableProperties();
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            valve.OnUpdate(time);
            UpdateAnimatableProperties();
        }

        protected override void OnSwitchChange()
        {
            valve.InputBinary(isOn, InputEffect.Timed);
        }

        protected virtual void UpdateAnimatableProperties()
        {
        }

#if UNITY_EDITOR
        public override void OnValidate()
        {
            valve.InputBinary(isOn, InputEffect.Immediate);
            UpdateAnimatableProperties();
        }
#endif
    }
}
