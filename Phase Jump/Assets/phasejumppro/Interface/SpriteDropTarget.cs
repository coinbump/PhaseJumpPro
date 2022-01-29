using UnityEngine;
using System.Collections;

namespace PJ
{
    /// <summary>
    /// Changes the sprite for an object when the drag is over it.
    /// </summary>
    public class SpriteDropTarget : DropTarget
    {
        public Sprite dragOverSprite;

        protected SpriteRenderer spriteRenderer;
        protected Sprite defaultSprite;

        public void Awake()
        {
            if (TryGetComponent(out SpriteRenderer spriteRenderer))
            {
                this.spriteRenderer = spriteRenderer;
                defaultSprite = spriteRenderer.sprite;
            }
        }

        public override void OnDragEnter(DragItems items)
        {
            if (null == spriteRenderer)
            {
                return;
            }

            spriteRenderer.sprite = dragOverSprite;
        }

        public override void OnDragLeave()
        {
            if (null == spriteRenderer)
            {
                return;
            }

            spriteRenderer.sprite = defaultSprite;
        }
    }
}
