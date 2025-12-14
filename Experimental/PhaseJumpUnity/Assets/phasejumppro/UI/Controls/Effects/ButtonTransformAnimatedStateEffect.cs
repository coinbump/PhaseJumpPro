using System.Collections.Generic;
using System;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 6/10/23
*/
namespace PJ
{
    /// <summary>
    /// Changes the transform based on the button state
    /// </summary>
    public class ButtonTransformAnimatedStateEffect : SomeTransformAnimatedStateEffect<ButtonControl.StateType>
    {
        /// <summary>
        /// Unparented transforms can cause problems if we try to modify the position.
        /// If true, do not modify the position.
        /// </summary>
        public bool ignorePosition = false;

        protected override void OnAnimatedValueChange()
        {
            var value = AnimatedValue;

            if (!ignorePosition)
            {
                gameObject.transform.localPosition = value.position;
            }
            gameObject.transform.localScale = value.scale;
            gameObject.transform.eulerAngles = value.rotation;
        }
    }
}
