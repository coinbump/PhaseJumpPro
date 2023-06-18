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
    /// Changes the sprite based on the button state
    /// </summary>
    public class ButtonSpriteStateEffect : SomeSpriteStateEffect<ButtonControl.StateType>
    {
        protected override void OnValueChange()
        {
            if (TryGetComponent(out SpriteRenderer spriteRenderer))
            {
                if (value)
                {
                    spriteRenderer.sprite = value;
                }
                else
                {
                    spriteRenderer.sprite = defaultValue;
                }
            }
        }
    }
}
