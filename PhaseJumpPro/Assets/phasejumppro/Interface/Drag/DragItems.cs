using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple struct
 * CODE REVIEW: 11/13/22
 */
namespace PJ
{
    /// <summary>
    /// Collection of things we're dragging.
    /// FUTURE: support item providers if needed for non-immediate data
    /// </summary>
    public struct DragItems
    {
        public List<Draggable2D> draggables;

        public DragItems(List<Draggable2D> draggables)
        {
            this.draggables = draggables;
        }
    }
}
