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
    /// A focus effect is applied when an object has focus
    /// </summary>
    public abstract class SomeFocusEffect : MonoBehaviour
    {
        public bool hasFocus = false;

        public bool HasFocus
        {
            get
            {
                return hasFocus;
            }
            set
            {
                if (hasFocus == value)
                {
                    return;
                }

                hasFocus = value;
                OnFocusChange();
            }
        }

        protected abstract void OnFocusChange();
        public virtual void OnValidate() { }
    }

    /// <summary>
    /// Uses a valve to perform animated changes when focus changes
    /// When this object receives focus, the valve is opened over N time
    /// Check the valve's state to find a normalized value (0-1.0) to use for animations
    /// </summary>
    public class AnimatedFocusEffect : SomeFocusEffect
    {
        public float focusInTime = 0.3f;
        public float focusOutTime = 0.3f;

        public float FocusInTime
        {
            get => focusInTime;
            set
            {
                focusInTime = value;

                if (null == focusValve.turnOnTimer) { return; }
                focusValve.turnOnTimer.duration = value;
            }
        }

        public float FocusOutTime
        {
            get => focusOutTime;
            set
            {
                focusOutTime = value;

                if (null == focusValve.turnOffTimer) { return; }
                focusValve.turnOffTimer.duration = value;
            }
        }

        /// <summary>
        /// Used to animate changes in focus over time
        /// </summary>
        protected Valve focusValve = new Valve();

        protected override void Awake()
        {
            base.Awake();

            focusValve.turnOffTimer = new TransformTimer(new InterpolateOutSquared(), focusOutTime, SomeTimed.RunType.RunOnce);
            focusValve.turnOnTimer = new TransformTimer(new InterpolateOutSquared(), focusInTime, SomeTimed.RunType.RunOnce);
        }

        protected override void Start()
        {
            base.Start();

            focusValve.InputBinary(hasFocus, InputEffect.Immediate);
        }

        protected override void Update()
        {
            base.Update();

            focusValve.OnUpdate(new TimeSlice(delta: Time.deltaTime));
        }

        protected override void OnFocusChange()
        {
            focusValve.InputBinary(hasFocus, InputEffect.Timed);
        }

        protected virtual void UpdateAnimatableProperties()
        {
        }

#if UNITY_EDITOR
        public override void OnValidate()
        {
            focusValve.InputBinary(hasFocus, InputEffect.Immediate);
            UpdateAnimatableProperties();
        }
#endif
    }
}
