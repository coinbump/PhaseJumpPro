using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Allows an object to have focus, and animate in/out of focus.
    /// </summary>
    public class Focusable : MonoBehaviour
    {
        [SerializeField]
        private bool hasFocus = false;

        public float focusInTime = 0.3f;
        public float focusOutTime = 0.3f;

        /// <summary>
        /// Used to animate changes in focus over time
        /// </summary>
        protected Valve focusValve = new Valve();

        public bool HasFocus
        {
            get
            {
                return hasFocus;
            }
            set
            {
                if (hasFocus == value) {
                    return;
                }

                hasFocus = value;
                OnFocusChange();
            }
        }

        protected virtual void OnFocusChange()
        {
            focusValve.InputBinary(hasFocus, InputEffect.Timed);
        }

        protected virtual void Awake()
        {
            focusValve.turnOffTimer = new TransformTimer(new InterpolateOutSquared(), focusOutTime, SomeTimed.RunType.RunOnce);
            focusValve.turnOnTimer = new TransformTimer(new InterpolateOutSquared(), focusInTime, SomeTimed.RunType.RunOnce);
        }

        protected virtual void Start()
        {
            focusValve.InputBinary(hasFocus, InputEffect.Immediate);
        }

        protected virtual void Update()
        {
            focusValve.OnUpdate(new TimeSlice(delta: Time.deltaTime));
        }
    }
}
