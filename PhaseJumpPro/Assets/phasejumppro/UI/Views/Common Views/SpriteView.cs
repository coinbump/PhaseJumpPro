using System;
using UnityEngine;

/*
RATING: 5 stars
Simple type
CODE REVIEW: 6/10/23
*/
namespace PJ
{
    /// <summary>
    /// View that fits to the size of its sprite (or first child with sprite)
    /// </summary>
    public class SpriteView : View2D
    {
        public SpriteView()
        {
            viewModifier = new SpriteViewModifier2D();
        }

        protected void Reset()
        {
            isFixedWidth = true;
            isFixedHeight = true;
        }
    }
}
