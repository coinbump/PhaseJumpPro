#pragma once

#include "SomeResolver.h"
#include "SomeSignal.h"
#include "UIWorldSystem.h"
#include "WorldComponent.h"

// Future Feature, Unfinished, don't use
namespace PJ {
    /// Hints for changing focus in response to focus navigation events
    struct FocusHint {
        // TODO: should these be funcs?... Yes, but store the WP
        WP<WorldNode> left;
        WP<WorldNode> right;
        WP<WorldNode> up;
        WP<WorldNode> down;
        WP<WorldNode> previous;
        WP<WorldNode> next;
    };

    /// Focus  coordinator event for determining current focus
    class FocusEvent : public SomeSignal {
    public:
        String id;
    };

    namespace FocusEventId {
        auto constexpr Next = "next";
        auto constexpr Previous = "previous";
        auto constexpr Left = "left";
        auto constexpr Right = "right";
        auto constexpr Up = "up";
        auto constexpr Down = "down";
    } // namespace FocusEventId

    /// Handles focus behavior and state for object
    /// Each object that can receive focus must have one
    class FocusHandler : public WorldComponent<> {
    public:
        using Base = WorldComponent;

    protected:
        bool hasFocus = false;
        bool isFocusable = true;

    public:
        FocusHint hint;
    };

    class SomeFocusCoordinator {
    public:
        virtual ~SomeFocusCoordinator() {}

        virtual SP<WorldNode> Evaluate(SP<FocusHandler> activeFocus, FocusEvent event) = 0;
    };

    class FocusHintCoordinator : public SomeFocusCoordinator {
    public:
        // MARK: SomeFocusCoordinator

        SP<WorldNode> Evaluate(SP<FocusHandler> activeFocus, FocusEvent event) override {
            GUARDR(activeFocus, nullptr)

            auto& hint = activeFocus->hint;

            if (event.id == FocusEventId::Next) {
                auto lock = hint.next.lock();
                if (lock) {
                    return lock;
                }
            }
            // FUTURE: add the rest as needed

            return nullptr;
        }
    };

    /// Allows an object to have focus
    //    class FocusHandler : public WorldComponent<>

    //    public:
    //        int focusPriority = 0;
    //        SP<SomeEffect> focusEffect;
    //
    //        SP<SomeResolver<SP<UISystem>>>
    //        uiSystemResolver =
    //        MAKE<UISystemSharedResolver>();
    //
    //        virtual SP<UISystem> UISystem() const
    //        {
    //            return uiSystemResolver->Resolve();
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
    //        virtual VectorList<SP<FocusHandler>> SiblingFocusables(bool
    //        includeThis)
    //        {
    //            auto focusable = GetComponent<FocusHandler>();
    //
    //            VectorList<SP<FocusHandler>> result;
    //
    //            auto parent =
    //            focusable->owner().lock()->transform.parent.gameObject;
    //
    //            foreach (Transform siblingTransform in parent.transform)
    //            {
    //                if (siblingTransform->owner().lock() == focusable &&
    //                !includeThis)
    //                {
    //                    continue;
    //                }
    //
    //                auto thisFocusable =
    //                siblingTransform.GetComponent<FocusHandler>(); if (null ==
    //                thisFocusable) { continue; }
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
} // namespace PJ
