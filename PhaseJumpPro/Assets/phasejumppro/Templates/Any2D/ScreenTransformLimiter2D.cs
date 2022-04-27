using UnityEngine;
using System.Collections;

namespace PJ
{
    /// <summary>
    /// Limits the transform to the visible orthographic bounds of the screen
    /// Useful for fullscreen games, with no exterior UI
    /// </summary>
    // LIMITATIONS: this doesn't take into account pillar box or letterbox bars
    public class ScreenTransformLimiter2D : SomeRectTransformLimiter2D
    {
        protected Optional<Vector2> size;
            
        public override Vector2 Size
        {
            get
            {
                if (null != size)
                {
                    return size.value;
                }

                var screenWorldSize = Utils.ScreenWorldSize();
                
                //Debug.Log("Limiter X Size: " + screenWorldSize.ToString() + " ySize: " + screenWorldSize.ToString());
                
                size = new Optional<Vector2>(screenWorldSize);
                return size.value;
            }
        }
    }
}
