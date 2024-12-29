#pragma once

#include "List.h"
#include "SomePosition.h"
#include "SomeSignal.h"
#include "Tags.h"
#include "UnorderedSet.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/22/24
 */
namespace PJ {
    /// Sent after the window is resized
    class WindowResizeUIEvent : public SomeSignal {
    public:
        /// Logical size (not pixel size) of the window
        Vector2 size;

        WindowResizeUIEvent(Vector2 size) :
            size(size) {}
    };

    /// Shared pointer list so we can use filters and make copies
    using UIEventList = VectorList<SP<SomeSignal>>;
} // namespace PJ
