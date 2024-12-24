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
    /// A user input event object
    class SomeUIEvent : public SomeSignal {
    public:
        /// Metadata for event
        Tags tags;

        virtual ~SomeUIEvent() {}
    };

    /// Shared pointer list so we can use filters and make copies
    using UIEventList = VectorList<SP<SomeUIEvent>>;
} // namespace PJ
