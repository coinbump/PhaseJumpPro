using UnityEngine;

/*
 * RATING: 5 stars
 * Simple animation utility
 * CODE REVIEW: 12/18/22
 */
namespace PJ
{
    /// <summary>
    /// A switch can be turned on or off
    /// </summary>
    public class Switch : Switchable
    {
        protected bool isOn = false;

        public bool IsOn
        {
            get
            {
                return isOn;
            }
            set
            {
                if (isOn == value)
                {
                    return;
                }

                isOn = value;
                OnSwitchChange();
            }
        }

        public virtual void OnSwitchChange() { }
    }
}
