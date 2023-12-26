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
    /// Changes color based on the button state
    /// </summary>
    public class ButtonColorStateEffect : SomeColorStateEffect<ButtonControl.StateType>
    {
        protected override void OnValueChange()
        {
            RendererTool.Color = value;
        }
    }
}
