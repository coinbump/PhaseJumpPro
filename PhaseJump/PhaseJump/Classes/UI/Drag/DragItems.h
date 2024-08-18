#ifndef PJDRAGITEMS_H
#define PJDRAGITEMS_H

#include "Utils.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 5/9/23
 */
namespace PJ {
    class SomeDragHandler;

    /// Collection of things we're dragging.
    /// FUTURE: support item providers if needed for non-immediate data
    struct DragItems {
    public:
        VectorList<SP<SomeDragHandler>> dragHandlers;

        DragItems() {}

        DragItems(VectorList<SP<SomeDragHandler>> const& dragHandlers) :
            dragHandlers(dragHandlers) {}
    };
} // namespace PJ

#endif
