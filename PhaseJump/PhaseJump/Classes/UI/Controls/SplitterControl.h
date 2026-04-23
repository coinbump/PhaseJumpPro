#pragma once

#include "ObservedValue.h"
#include "View2D.h"

/*
 RATING: 4 stars
 Tested and works
 CODE REVIEW: 4/22/26
 */
namespace PJ {
    /// A draggable handle that adjusts the first-child ratio of a SplitViewLayout. The ratio
    /// is observable so layout code can re-read it when the user drags the control.
    class SplitterControl : public View2D {
    public:
        using Base = View2D;
        using This = SplitterControl;

        /// Fraction (0..1) of the parent split's axis length given to the first child.
        ObservedValue<float> firstRatio{ 0.5f };

        SplitterControl() = default;

        explicit SplitterControl(float initialRatio) :
            firstRatio(initialRatio) {}

        float FirstRatio() const {
            return firstRatio.Value();
        }

        void SetFirstRatio(float value) {
            firstRatio = value;
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SplitterControl";
        }
    };
} // namespace PJ
