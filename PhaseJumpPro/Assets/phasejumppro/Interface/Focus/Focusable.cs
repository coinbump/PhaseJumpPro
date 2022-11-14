using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Standard interface pattern. Could use example scenes
 * CODE REVIEW: 11/13/22
 */
namespace PJ
{
    /// <summary>
    /// Allows an object to have focus
    /// </summary>
    public class Focusable : MonoBehaviour
    {
        [SerializeField]
        protected bool hasFocus = false;

        protected bool canBecomeFocused = true;

        // FUTURE: protected int focusGroupPriority = 0;

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

        protected override void Awake()
        {
            base.Awake();

            UpdateFocusEffect();
        }

        public virtual Focusable ParentFocusable()
        {
            return ParentComponent<Focusable>();
        }

        public virtual List<Focusable> SiblingFocusables(bool includeThis)
        {
            var focusable = GetComponent<Focusable>();

            List<Focusable> result = new();

            var parent = focusable.gameObject.transform.parent.gameObject;

            foreach (Transform siblingTransform in parent.transform)
            {
                if (siblingTransform.gameObject == focusable && !includeThis)
                {
                    continue;
                }

                var thisFocusable = siblingTransform.GetComponent<Focusable>();
                if (null == thisFocusable) { continue; }

                result.Add(thisFocusable);
            }

            return result;
        }

        protected void UpdateFocusEffect()
        {
            var focusEffect = GetComponent<SomeFocusEffect>();
            if (focusEffect)
            {
                focusEffect.HasFocus = HasFocus;
            }
        }

        protected virtual void OnFocusChange()
        {
            UpdateFocusEffect();

            UISystem.shared.UpdateFocusFor(this, hasFocus);
        }

#if UNITY_EDITOR
        protected virtual void OnValidate()
        {
            UpdateFocusEffect();

            var focusEffect = GetComponent<SomeFocusEffect>();
            if (focusEffect)
            {
                focusEffect.OnValidate();
            }
        }
#endif
    }
}
