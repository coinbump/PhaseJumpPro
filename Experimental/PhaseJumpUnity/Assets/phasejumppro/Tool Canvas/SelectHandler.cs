using UnityEngine;
using System;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 1/14/23
*/
namespace PJ
{
    public class SelectHandler : WorldComponent
    {
        [SerializeField]
        protected bool isSelected = false;

        protected bool isSelectable = true;

        public SomeEffect selectEffect;

        public bool IsSelectable => IsSelectable;

        public SomeDependencyResolver<UISystem> uiSystemDependencyResolver = new UISystemSharedDependencyResolver();

        public UISystem UISystem => uiSystemDependencyResolver.Dependency();

        public bool IsSelected
        {
            get => isSelected;
            set
            {
                if (isSelected == value)
                {
                    return;
                }
                if (value && !isSelectable)
                {
                    return;
                }

                isSelected = value;
                OnSelectChange();
            }
        }

        protected override void Awake()
        {
            base.Awake();

            UpdateSelectEffect();
        }

        protected void UpdateSelectEffect()
        {
            if (!selectEffect) { return; }
            selectEffect.IsOn = IsSelected;
        }

        protected virtual void OnSelectChange()
        {
            UpdateSelectEffect();

            var uiSystem = UISystem;
            if (null != uiSystem)
            {
                UISystem.UpdateSelectionFor(this);
            }

            if (TryGetComponent(out SomeNode node))
            {
                if (isSelected)
                {
                    node.stateTags.Add("selected");
                }
                else
                {
                    node.stateTags.Remove("selected");
                }
            }
        }

#if UNITY_EDITOR
        protected virtual void OnValidate()
        {
            UpdateSelectEffect();

            if (selectEffect)
            {
                selectEffect.UpdateEffectProperties();
            }
        }
#endif
    }
}