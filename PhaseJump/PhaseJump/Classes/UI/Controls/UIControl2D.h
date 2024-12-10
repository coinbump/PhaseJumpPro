#pragma once

#include "View2D.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// An interactive UI object that accepts inputs from the user and produces
    /// events. Example: button, switch, checkbox, etc.
    class UIControl2D : public View2D {
    public:
        using Base = View2D;
        using This = UIControl2D;

        using OnControlChangeFunc = std::function<void(This&)>;

    protected:
        /// Called when the control's state changes
        OnControlChangeFunc onControlChangeFunc;

    public:
        void SetOnControlChangeFunc(OnControlChangeFunc value) {
            onControlChangeFunc = value;
            GUARD(onControlChangeFunc)
            onControlChangeFunc(*this);
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "UIControl2D";
        }

    protected:
        void Awake() override;

        /// Called when the control state changes
        virtual void OnControlChange();
    };
} // namespace PJ
