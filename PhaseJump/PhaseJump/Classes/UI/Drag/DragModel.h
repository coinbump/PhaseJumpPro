#pragma once

#include "Utils.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/22/24
 */
namespace PJ {
    class DragHandler;

    /// Information needed for a drag gesture
    class DragModel {
    public:
        using DragHandlerList = VectorList<SP<DragHandler>>;

        // FUTURE: support item providers for on demand data
        DragHandlerList dragHandlers;

        DragModel(SP<DragHandler> dragHandler) {
            dragHandlers.push_back(dragHandler);
        }

        SP<DragHandler> DragHandler() const {
            return SafeFirst(dragHandlers);
        }
    };
} // namespace PJ
