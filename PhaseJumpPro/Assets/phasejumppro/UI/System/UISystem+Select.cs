using System;
using System.Collections.Generic;

/*
RATING: 5 stars
Has Unit Tests
CODE REVIEW: 6/10/23
*/
namespace PJ
{
    /// <summary>
    /// Manages selection state for objects
    /// </summary>
    public partial class UISystem
    {
        protected HashSet<SelectHandler> selection = new();

        public HashSet<SelectHandler> Selection
        {
            get => selection;
            set
            {
                HashSet<SelectHandler> removedSelectHandlers = new(selection);
                removedSelectHandlers.ExceptWith(value);

                foreach (var selectHandler in removedSelectHandlers)
                {
                    selectHandler.IsSelected = false;
                }

                foreach (var selectHandler in value)
                {
                    selectHandler.IsSelected = true;
                }

                this.selection = value;
            }
        }

        public void UpdateSelectionFor(SelectHandler selectHandler)
        {
            if (selectHandler.IsSelected)
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
