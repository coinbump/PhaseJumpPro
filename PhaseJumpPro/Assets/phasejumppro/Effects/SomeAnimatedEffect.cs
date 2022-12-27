using System.Collections;
using System.Collections.Generic;
using PJ;
using UnityEngine;

/*
 * RATING: 4 stars
 * Could use unit tests
 * CODE REVIEW: 12/26/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Uses a valve to perform animated changes when focus changes
    /// When this object receives focus, the valve is opened over N time
    /// Check the valve's state to find a normalized value (0-1.0) to use for animations
    /// </summary>
    public abstract class SomeAnimatedEffect : SomeEffect
    {
        protected float turnOnTime = 0.3f;
        protected float turnOffTime = 0.3f;

        public float TurnOnTime
        {
            get => turnOnTime;
            set
            {
                turnOnTime = value;
                valve.turnOnTimer.duration = value;
            }
        }

        public float TurnOffTime
        {
            get => turnOffTime;
            set
            {
                turnOffTime = value;
                valve.turnOffTimer.duration = value;
            }
        }

        /// <summary>
        /// Used to animate changes in focus over time
        /// </summary>
        protected Valve valve = new Valve();

        public SomeAnimatedEffect()
        {
            valve.turnOffTimer = new TransformTimer(turnOffTime, SomeRunner.RunType.RunOnce, new EaseOutSquared());
            valve.turnOnTimer = new TransformTimer(turnOnTime, SomeRunner.RunType.RunOnce, new EaseOutSquared());
        }

        protected override void Awake()
        {
            base.Awake();

            valve.turnOffTimer.duration = turnOffTime;
            valve.turnOnTimer.duration = turnOnTime;
        }

        protected override void Start()
        {
            base.Start();

            valve.InputBinary(isOn, InputEffect.Immediate);
            UpdateEffectProperties();
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            valve.OnUpdate(time);
            UpdateEffectProperties();
        }

        protected override void OnSwitchChange()
        {
            valve.InputBinary(isOn, InputEffect.Timed);
        }

#if UNITY_EDITOR
        protected override void OnValidate()
        {
            valve.InputBinary(isOn, InputEffect.Immediate);
            UpdateEffectProperties();
        }
#endif
    }
}
