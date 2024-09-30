#pragma once

#include "View2D.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/22/24
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
        /// Called when the control's state changes
        OnControlUpdateFunc onControlUpdateFunc;

    public:
        void SetOnControlUpdateFunc(OnControlUpdateFunc value) {
            onControlUpdateFunc = value;
            GUARD(onControlUpdateFunc)
            onControlUpdateFunc(*this);
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
