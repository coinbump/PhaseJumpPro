#pragma once

#include "Utils.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/22/24
 */
namespace PJ {
    class SomeDragHandler;

    /// Information needed for a drag gesture
    class DragModel {
    public:
        using DragHandlerList = VectorList<SP<SomeDragHandler>>;

        // FUTURE: support item providers for on demand data
        DragHandlerList dragHandlers;

        DragModel(SP<SomeDragHandler> dragHandler) {
            dragHandlers.push_back(dragHandler);
        }

        SP<SomeDragHandler> DragHandler() const {
            return SafeFirst(dragHandlers);
        }
    };
} // namespace PJ
