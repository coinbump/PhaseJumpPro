#ifndef PJDRAGMODEL_H
#define PJDRAGMODEL_H

#include "Macros.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 5/9/23
 */
namespace PJ
{
    class SomeDragHandler;

    /// Information on initiating a drag
    class DragModel
    {
    public:
        // We don't expect the dragged object to be destroyed during the drag
        // if you want to destroy it, cancel the drag first
        SP<SomeDragHandler> dragHandler;

        DragModel(SP<SomeDragHandler> dragHandler) : dragHandler(dragHandler)
        {
        }
    };
}

#endif
