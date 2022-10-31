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
        protected bool hasFocus = false;

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

        public virtual Focusable ParentFocusable()
        {
            var parent = transform.parent.gameObject;
            return parent.GetComponent<Focusable>();
        }

        public virtual List<Focusable> SiblingFocusables(bool includeThis)
        {
            var focusable = GetComponent<Focusable>();

            List<Focusable> result = new();

            var parent = focusable.gameObject.transform.parent.gameObject;

            foreach (Transform siblingTransform in parent.transform)
            {
                if (siblingTransform.gameObject == focusable && !includeThis)
                {
                    continue;
                }

                var thisFocusable = siblingTransform.GetComponent<Focusable>();
                if (null == thisFocusable) { continue; }

                result.Add(thisFocusable);
            }

            return result;
        }

        protected virtual void OnChildFocusChange(Focusable focusable)
        {
            var parentFocusable = ParentFocusable();
            if (null == parentFocusable) { return; }
            
            parentFocusable.OnChildFocusChange(this);
        }

        protected virtual void OnFocusChange()
        {
            var parentFocusable = ParentFocusable();
            if (null != parentFocusable)
            {
                parentFocusable.OnChildFocusChange(this);
            }
        }

        protected virtual void Awake()
        {
        }

        protected virtual void Start()
        {
        }

        protected virtual void Update()
        {
        }
    }

    public class AnimatedFocusable : Focusable
    {
        public float focusInTime = 0.3f;
        public float focusOutTime = 0.3f;

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

        protected virtual void OnFocusChange()
        {
            base.OnFocusChange();

            focusValve.InputBinary(hasFocus, InputEffect.Timed);
        }
    }
}
