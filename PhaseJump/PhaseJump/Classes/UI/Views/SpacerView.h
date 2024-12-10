#pragma once

#include "View2D.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 11/25/24
 */
namespace PJ {
    /// View with no intrinsic size used for spacing views in supported layouts
    /// Supported layouts use this as a special case for sizing (HStack, VStack) and standard UI
    /// constraints are ignored
    class SpacerView : public View2D {
    public:
        SpacerView() {}

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SpacerView";
        }
    };
} // namespace PJ
