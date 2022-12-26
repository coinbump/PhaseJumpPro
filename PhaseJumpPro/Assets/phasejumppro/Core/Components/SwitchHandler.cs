using UnityEngine;

/*
RATING: 5 stars
Simple pattern
CODE REVIEW: 12/19/22
*/
namespace PJ
{
    public class SwitchHandler : WorldComponent, Switchable
    {
        [SerializeField]
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

        protected virtual void OnSwitchChange() { }

        public virtual void OnValidate()
        {
            OnSwitchChange();
        }
    }
}
