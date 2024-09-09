#pragma once

#include "SomeHoverGestureHandler.h"
#include "View2D.h"

/*
 RATING: 5 stars
 Verified
 CODE REVIEW: 9/4/24
 */
namespace PJ {
    /// An interactive UI object that accepts inputs from the user and produces
    /// events. Example: button, switch, checkbox, etc.
    class UIControl2D : public View2D {
    public:
        using Base = View2D;
        using This = UIControl2D;

        using OnControlUpdateFunc = std::function<void(This&)>;

    protected:
        class HoverGestureHandler : public SomeHoverGestureHandler {
        public:
            WP<UIControl2D> target;

            HoverGestureHandler(WP<UIControl2D> target) :
                target(target) {}

            void SetIsHovering(bool value) override {
                GUARD(!target.expired())
                target.lock()->isHovering = value;
            }

            // MARK: SomeWorldComponent

            String TypeName() const override {
                return "UIControl2D- HoverGestureHandler";
            }
        };

        /// Called when the control's state changes
        OnControlUpdateFunc onControlUpdateFunc;

        bool isHovering = false;

    public:
        void SetOnControlUpdateFunc(OnControlUpdateFunc value) {
            onControlUpdateFunc = value;
            GUARD(onControlUpdateFunc)
            onControlUpdateFunc(*this);
        }

        void SetIsHovering(bool value) {
            GUARD(isHovering != value)
            isHovering = value;
            OnStateChange();
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "UIControl2D";
        }

    protected:
        void Awake() override;

        /// Called when the control state changes
        virtual void OnStateChange();
    };
} // namespace PJ
