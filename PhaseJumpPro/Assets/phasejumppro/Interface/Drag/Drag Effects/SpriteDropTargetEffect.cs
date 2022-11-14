using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 11/13/22
 */
namespace PJ
{
    /// <summary>
    /// Change the sprite for a drop target when the drag is over it.
    /// </summary>
    public class SpriteDropTargetEffect : SomeDropTargetEffect
    {
        public Sprite dragOverSprite;

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

        protected override void OnStateChange()
        {
            switch (state)
            {
                case DropTarget.StateType.DragOverAccept:
                    spriteRenderer.sprite = dragOverSprite;
                    break;
                default:
                    spriteRenderer.sprite = defaultSprite;
                    break;
            }
        }
    }
}
