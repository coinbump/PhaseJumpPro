#pragma once

#include "Binding.h"
#include "UIControl2D.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/14/24
 */
namespace PJ {
    /// Handles button tracking behavior for a button control
    class DialControl : public UIControl2D {
    public:
        using This = DialControl;
        using Base = UIControl2D;

    protected:
        float fullDragDelta{};
        float radius{};
        Binding<float> valueBinding;
        float dragStartValue{};

    public:
        struct Config {
            float radius{};

            /// The # of screen coordinates required to drag to move the value from 0 to 1
            float fullDragDelta = 200;

            Binding<float> valueBinding;
        };

        DialControl(Config const& config);

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "DialControl";
        }

    protected:
        void Awake() override;
    };
} // namespace PJ
