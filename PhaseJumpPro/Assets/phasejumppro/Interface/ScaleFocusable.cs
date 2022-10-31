using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
    public class ScaleFocusable : AnimatedFocusable
    {
        public float offScale = 1.0f;
        public float onScale = 2.0f;

        protected override void Start()
        {
            base.Start();

            UpdateScale();
        }

        protected override void Update()
        {
            base.Update();

            UpdateScale();
        }

        protected void UpdateScale()
        {
            var valveState = focusValve.ValveState;
            var transformScale = offScale + (onScale - offScale) * valveState;
            transform.localScale = new Vector3(transformScale, transformScale, transformScale);
        }
    }
}
