using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Sizes button to its sprite
    /// </summary>
    public class SpriteButtonControl : ButtonControl
    {
        public SpriteButtonControl()
        {
            viewModifier = new SpriteViewModifier2D();
        }
    }
}
