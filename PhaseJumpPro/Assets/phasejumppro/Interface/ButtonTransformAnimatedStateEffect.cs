using System.Collections.Generic;
using System;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Changes the transform based on the button state
    /// </summary>
    public class ButtonTransformAnimatedStateEffect : SomeTransformAnimatedStateEffect<ButtonControl.StateType>
    {
        protected override void OnAnimatedValueChange()
        {
            var value = AnimatedValue;
            gameObject.transform.localPosition = value.position;
            gameObject.transform.localScale = value.scale;
            gameObject.transform.eulerAngles = value.rotation;
        }
    }
}
