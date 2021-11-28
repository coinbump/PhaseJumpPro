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
                EvtFocusChanged();
            }
        }

        protected virtual void EvtFocusChanged()
        {
            focusValve.InputBinary(hasFocus, InputEffect.OverTime);
        }

        protected virtual void Awake()
        {
            focusValve.turnOffTimer = new InterpolateTimer(new InterpolateOutSquared(), focusOutTime, SomeTimed.Type.Persistent);
            focusValve.turnOnTimer = new InterpolateTimer(new InterpolateOutSquared(), focusInTime, SomeTimed.Type.Persistent);
        }

        protected virtual void Start()
        {
            focusValve.InputBinary(hasFocus, InputEffect.Immediate);
        }

        protected virtual void Update()
        {
            focusValve.EvtUpdate(new TimeSlice(delta: Time.deltaTime));
        }
    }
}
