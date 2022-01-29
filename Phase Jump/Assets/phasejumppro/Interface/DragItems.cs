using System.Collections;
using System.Collections.Generic;
using UnityEngine;

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
