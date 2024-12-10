#pragma once

#include "DesignSystem.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/8/24
 */
namespace PJ {
    /// Simple, basic design system ("ugly duckling" UI)
    class DuckDesignSystem : public DesignSystem {
    public:
        /// OPTIMIZE: by default, we assume that all shapes are opaque and behind transparent text
        /// with this design system
        bool areShapesOpaque = true;

        using Base = DesignSystem;
        using This = DuckDesignSystem;

        DuckDesignSystem();
    };
} // namespace PJ
