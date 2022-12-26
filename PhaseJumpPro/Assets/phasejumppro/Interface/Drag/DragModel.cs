using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple value types
 * CODE REVIEW: 11/13/22
 */
namespace PJ
{
    /// <summary>
    /// Information on initiating a drag.
    /// </summary>
    public class DragModel
    {
        public SomeDragHandler dragHandler;

        public DragModel(SomeDragHandler dragHandler)
        {
            this.dragHandler = dragHandler;
        }
    }
}
