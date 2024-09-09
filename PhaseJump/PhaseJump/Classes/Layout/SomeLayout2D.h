#pragma once

#include "SomeLayout.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/5/24
 */
namespace PJ {
    /// Specialization for 2D layouts
    class SomeLayout2D : public SomeLayout {
        void ApplyLayout() override {}
    };
} // namespace PJ
