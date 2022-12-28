using System;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// A button that, when pressed, toggles off and on
    /// </summary>
    public class ToggleButton : Button
    {
        public bool isToogleOn = false;
        public SomeEffect toggleEffect;

        public bool IsToggleOn
        {
            get => isToogleOn;
            set
            {
                if (isToogleOn == value) { return; }
                isToogleOn = value;
                OnToggleChange();
            }
        }

        protected override void Awake()
        {
            base.Awake();

            UpdateToggle();
        }

        protected override void OnPress()
        {
            base.OnPress();

            IsToggleOn = !IsToggleOn;
        }

        protected void OnToggleChange()
        {
            UpdateToggle();
        }

        protected void UpdateToggle()
        {
            if (!toggleEffect) { return; }
            toggleEffect.IsOn = isToogleOn;
        }
    }
}
