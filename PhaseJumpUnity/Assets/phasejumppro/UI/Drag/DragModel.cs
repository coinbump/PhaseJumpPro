using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple value types
 * CODE REVIEW: 6/10/23
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Information on initiating a drag
    /// </summary>
    public class DragModel
    {
        // We don't expect the dragged object to be destroyed during the drag
        // if you want to destroy it, cancel the drag first
        public SomeDragHandler dragHandler;

        public DragModel(SomeDragHandler dragHandler)
        {
            this.dragHandler = dragHandler;
        }
    }
}
