using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple struct
 * CODE REVIEW: 6/10/23
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Collection of things we're dragging.
    /// FUTURE: support item providers if needed for non-immediate data
    /// </summary>
    public struct DragItems
    {
        public List<SomeDragHandler> dragHandlers;

        public DragItems(List<SomeDragHandler> dragHandlers)
        {
            this.dragHandlers = dragHandlers;
        }
    }
}
