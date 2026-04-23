#pragma once

#include "DesignSystem.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/8/24
 */
namespace PJ {
    /**
     Simple, basic design system ("ugly duckling" UI)

     No transparency, no shadows, nothing fancy.
     */
    class DuckDesignSystem : public DesignSystem {
    public:
        /// Optimize: by default, we assume that all shapes are opaque and behind transparent text
        /// with this design system
        bool areShapesOpaque = true;

        using Base = DesignSystem;
        using This = DuckDesignSystem;

        DuckDesignSystem();

    private:
        void RegisterSurface();
        void RegisterButton();
        void RegisterSlider();
        void RegisterSegmentToggle();
        void RegisterImageToggle();
        void RegisterSwitchToggle();
        void RegisterCheckTypeButtons();
        void RegisterProgressBar();
        void RegisterProgressCircle();
        void RegisterDial();
        void RegisterLabel();
        void RegisterToast();
        void RegisterToolTip();
        void RegisterSplitter();
    };
} // namespace PJ
