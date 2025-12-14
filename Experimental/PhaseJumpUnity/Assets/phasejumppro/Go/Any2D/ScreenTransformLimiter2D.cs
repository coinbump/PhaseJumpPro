using UnityEngine;
using System.Collections;

/*
 * RATING 5 stars
 * Tested and works
 * CODE REVIEW: 4/27/22
 */
namespace PJ
{
    /// <summary>
    /// Limits the transform to the visible orthographic bounds of the screen
    /// Useful for fullscreen games, with no exterior UI
    /// </summary>
    // LIMITATIONS: this doesn't take into account pillar box or letterbox bars
    public class ScreenTransformLimiter2D : SomeRectTransformLimiter2D
    {
        public Camera screenCamera;

        protected Optional<Vector2> size;
            
        public override Vector2 Size
        {
            get
            {
                if (null == screenCamera) { return Vector2.zero; }
                if (null != size)
                {
                    return size.value;
                }

                var screenWorldSize = Utils.ScreenWorldSize(screenCamera);
                
                //Debug.Log("Limiter X Size: " + screenWorldSize.ToString() + " ySize: " + screenWorldSize.ToString());
                
                size = new Optional<Vector2>(screenWorldSize);
                return size.value;
            }
        }
    }
}
