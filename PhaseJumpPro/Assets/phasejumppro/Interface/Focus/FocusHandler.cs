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
    public class FocusHandler : WorldComponent
    {
        [SerializeField]
        protected bool hasFocus = false;

        protected bool canBecomeFocused = true;

        public int focusPriority = 0;
        public SomeEffect focusEffect;

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

        public virtual FocusHandler ParentFocusable()
        {
            return ParentComponent<FocusHandler>();
        }

        public virtual List<FocusHandler> SiblingFocusables(bool includeThis)
        {
            var focusable = GetComponent<FocusHandler>();

            List<FocusHandler> result = new();

            var parent = focusable.gameObject.transform.parent.gameObject;

            foreach (Transform siblingTransform in parent.transform)
            {
                if (siblingTransform.gameObject == focusable && !includeThis)
                {
                    continue;
                }

                var thisFocusable = siblingTransform.GetComponent<FocusHandler>();
                if (null == thisFocusable) { continue; }

                result.Add(thisFocusable);
            }

            return result;
        }

        protected void UpdateFocusEffect()
        {
            if (!focusEffect) { return; }
            focusEffect.IsOn = HasFocus;
        }

        protected virtual void OnFocusChange()
        {
            UpdateFocusEffect();

            UISystem.shared.UpdateFocusFor(this, hasFocus);

            if (TryGetComponent(out SomeNode node))
            {
                if (hasFocus)
                {
                    node.stateTags.Add("focused");
                }
                else
                {
                    node.stateTags.Remove("focused");
                }
            }
        }

#if UNITY_EDITOR
        protected virtual void OnValidate()
        {
            UpdateFocusEffect();

            var focusEffect = GetComponent<SomeEffect>();
            if (focusEffect)
            {
                focusEffect.UpdateEffectProperties();
            }
        }
#endif
    }
}
