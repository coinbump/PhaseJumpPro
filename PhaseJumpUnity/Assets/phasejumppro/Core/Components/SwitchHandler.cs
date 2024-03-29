using UnityEngine;

/*
RATING: 5 stars
Simple pattern
CODE REVIEW: 12/26/22
PORTED TO: C++
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

        protected virtual void OnValidate()
        {
            OnSwitchChange();
        }
    }
}
