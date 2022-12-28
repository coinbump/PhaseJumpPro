using System.Collections.Generic;
using System;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Changes the sprite based on the button state
    /// </summary>
    public class ButtonSpriteStateEffect : SomeSpriteStateEffect<Button.StateType>
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
