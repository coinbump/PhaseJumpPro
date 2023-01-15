using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 1/14/23
*/
namespace PJ
{
    /// <summary>
    /// Manages selection state for objects
    /// </summary>
    public partial class UISystem
    {
        protected HashSet<SelectHandler> selection = new();

        public void SetSelection(HashSet<SelectHandler> selectHandlers)
        {
            foreach (var selectHandler in selection)
            {
                if (selectHandlers.Contains(selectHandler)) { continue; }
                selectHandler.IsSelected = false;
            }

            foreach (var selectHandler in selectHandlers)
            {
                selectHandler.IsSelected = true;
            }
        }

        public void UpdateSelectFor(SelectHandler selectHandler, bool isSelected)
        {
            if (isSelected)
            {
                selection.Add(selectHandler);
            }
            else
            {
                selection.Remove(selectHandler);
            }
        }
    }
}
