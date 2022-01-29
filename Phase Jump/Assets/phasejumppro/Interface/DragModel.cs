using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple value types
 * CODE REVIEW: 1/18/22
 */
namespace PJ
{
    /// <summary>
    /// Link to an object dragged along with the reference object that was tapped
    /// </summary>
    public class DragLink
    {
        public GameObject gameObject;
        public Vector3 offset;
    }

    /// <summary>
    /// Information on initiating a drag.
    /// </summary>
    public class DragModel
    {
        public Draggable2D dragged;
        public List<DragLink> dragLinks;

        public DragModel(Draggable2D dragged)
        {
            this.dragged = dragged;
        }

        public DragModel(Draggable2D dragged, List<DragLink> dragLinks)
        {
            this.dragged = dragged;
            this.dragLinks = dragLinks;
        }
    }
}
