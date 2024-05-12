#ifndef PJFOCUSHANDLER_H
#define PJFOCUSHANDLER_H

#include "WorldComponent.h"
#include "SomeEffect.h"
#include "SomeDependencyResolver.h"
#include "UISystem.h"

namespace PJ
{
    // TODO: needs more tech
    /// <summary>
    /// Allows an object to have focus
    /// </summary>
//    class FocusHandler : public WorldComponent
//    {
//    public:
//        using Base = WorldComponent;
//
//    protected:
//        bool hasFocus = false;
//        bool isFocusable = true;
//
//    public:
//        int focusPriority = 0;
//        SP<SomeEffect> focusEffect;
//
//        SP<SomeDependencyResolver<SP<UISystem>>> uiSystemDependencyResolver = MAKE<UISystemSharedDependencyResolver>();
//
//        virtual SP<UISystem> UISystem() const
//        {
//            return uiSystemDependencyResolver->Dependency();
//        }
//
//        bool HasFocus() const { return hasFocus }
//        void SetHasFocus(bool value) {
//            if (hasFocus == value)
//            {
//                return;
//            }
//
//            hasFocus = value;
//            OnFocusChange();
//        }
//
//    protected:
//        void Awake() override
//        {
//            Base::Awake();
//
//            UpdateFocusEffect();
//        }
//
//    public:
//        virtual FocusHandler ParentFocusable()
//        {
//            return ParentComponent<FocusHandler>();
//        }
//
//        virtual VectorList<SP<FocusHandler>> SiblingFocusables(bool includeThis)
//        {
//            auto focusable = GetComponent<FocusHandler>();
//
//            VectorList<SP<FocusHandler>> result;
//
//            auto parent = focusable->owner().lock()->transform.parent.gameObject;
//
//            foreach (Transform siblingTransform in parent.transform)
//            {
//                if (siblingTransform->owner().lock() == focusable && !includeThis)
//                {
//                    continue;
//                }
//
//                auto thisFocusable = siblingTransform.GetComponent<FocusHandler>();
//                if (null == thisFocusable) { continue; }
//
//                result.Add(thisFocusable);
//            }
//
//            return result;
//        }
//
//    protected:
//        void UpdateFocusEffect()
//        {
//            if (!focusEffect) { return; }
//            focusEffect.SetIsOn(HasFocus());
//        }
//
//        virtual void OnFocusChange()
//        {
//            UpdateFocusEffect();
//
//            UISystem()->UpdateFocusFor(this);
//        }
//    };
}

#endif
