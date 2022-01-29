using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Target that can accept drag items
    /// </summary>
    public class DropTarget : PJ.MonoBehaviour
    {
        public virtual bool CanAcceptDrag(DragItems items)
        {
            return true;
        }

        public virtual void OnDragEnter(DragItems items)
        {

        }

        public virtual void OnDragLeave()
        {

        }
    }
}
