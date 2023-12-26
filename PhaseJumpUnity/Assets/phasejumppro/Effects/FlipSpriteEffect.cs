using UnityEngine;

/*
RATING: 5 STARS
Tested and works
CODE REVIEW: 12/27/22
*/
namespace PJ
{
    /// <summary>
    /// Flip the sprite
    /// </summary>
    public class FlipSpriteEffect : SomeEffect
    {
        [Tooltip("If true, effect flips sprite horizontally")]
        public bool modifyFlipX = true;

        [Tooltip("If true, effect flips sprite vertically")]
        public bool modifyFlipY = false;

        public bool offFlipX = false;
        public bool offFlipY = false;

        public override void UpdateEffectProperties()
        {
            base.UpdateEffectProperties();

            if (TryGetComponent(out SpriteRenderer spriteRenderer))
            {
                if (modifyFlipX)
                {
                    spriteRenderer.flipX = offFlipX ? !IsOn : IsOn;
                }
                if (modifyFlipY)
                {
                    spriteRenderer.flipY = offFlipY ? !IsOn : IsOn;
                }
            }
        }
    }
}
