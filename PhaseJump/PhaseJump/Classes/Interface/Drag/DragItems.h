#ifndef PJDRAGITEMS_H
#define PJDRAGITEMS_H

#include "VectorList.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 5/9/23
 */
namespace PJ
{
    class SomeDragHandler;

    /// <summary>
    /// Collection of things we're dragging.
    /// FUTURE: support item providers if needed for non-immediate data
    /// </summary>
    struct DragItems
    {
    public:
        VectorList<SP<SomeDragHandler>> dragHandlers;

        DragItems() {
        }

        DragItems(VectorList<SP<SomeDragHandler>> const& dragHandlers) : dragHandlers(dragHandlers)
        {
        }
    };
}

#endif
