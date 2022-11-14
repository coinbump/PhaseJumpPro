using System.Collections;
using System.Collections.Generic;
using PJ;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple animation utility
 * CODE REVIEW: 11/13/22
 */
namespace PJ
{
    /// <summary>
    /// A focus effect is applied when an object has focus
    /// </summary>
    public abstract class SomeFocusEffect : MonoBehaviour
    {
        public bool hasFocus = false;

        public bool HasFocus
        {
            get
            {
                return hasFocus;
            }
            set
            {
                if (hasFocus == value)
                {
                    return;
                }

                hasFocus = value;
                OnFocusChange();
            }
        }

        protected abstract void OnFocusChange();
        public virtual void OnValidate() { }
    }
}
