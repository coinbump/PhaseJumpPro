using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 12/25/22
 */
namespace PJ
{
    /// <summary>
    /// Change the sprite
    /// </summary>
    public class ModifySpriteEffect : SomeEffect
    {
        public Sprite altSprite;

        protected SpriteRenderer spriteRenderer;
        protected Sprite defaultSprite;

        protected override void Awake()
        {
            base.Awake();

            if (TryGetComponent(out SpriteRenderer spriteRenderer))
            {
                this.spriteRenderer = spriteRenderer;
                defaultSprite = spriteRenderer.sprite;
            }
        }

        protected override void OnSwitchChange()
        {
            base.OnSwitchChange();

            switch (IsOn)
            {
                case true:
                    spriteRenderer.sprite = altSprite;
                    break;
                default:
                    spriteRenderer.sprite = defaultSprite;
                    break;
            }
        }
    }
}
