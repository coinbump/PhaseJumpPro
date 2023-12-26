using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 6/10/23
*/
namespace PJ
{
    /// <summary>
    /// Modifies standard view behavior
    /// </summary>
    public abstract class SomeViewModifier2D
    {
        public virtual Optional<float> IntrinsicWidth(View2D view) => null;
        public virtual Optional<float> IntrinsicHeight(View2D view) => null;
        public virtual void OnViewSizeChange(View2D view) { }
    }

    /// <summary>
    /// Adds sprite sizing to views by default
    /// </summary>
    public class SpriteViewModifier2D : SomeViewModifier2D
    {
        public SpriteRenderer SpriteRendererFor(View2D view)
        {
            // If the sprite is in the view itself, you won't be able to transform its position
            // If you need position-type animations, add the sprite as a child of the view
            if (view.TryGetComponent(out SpriteRenderer spriteRenderer))
            {
                return spriteRenderer;
            }

            // A sprite can be a child of the view, so that the sprite can be transformed as needed
            // without affecting view layout
            foreach (Transform childTransform in view.transform)
            {
                if (childTransform.gameObject.TryGetComponent(out SpriteRenderer childSpriteRenderer))
                {
                    return childSpriteRenderer;
                }
            }
            return null;
        }

        /// <summary>
        /// For tiled sprites, set the sprite size to the frame size
        /// </summary>
        public override void OnViewSizeChange(View2D view)
        {
            var spriteRenderer = SpriteRendererFor(view);
            if (null == spriteRenderer) { return; }

            var spriteSize = SpriteRendererSize(view, spriteRenderer);
            if (null != spriteSize)
            {
                spriteRenderer.size = spriteSize.value;
            }
        }

        public override Optional<float> IntrinsicWidth(View2D view)
        {
            var spriteRenderer = SpriteRendererFor(view);
            if (null == spriteRenderer) { return null; }

            switch (spriteRenderer.drawMode)
            {
                case SpriteDrawMode.Sliced:
                case SpriteDrawMode.Tiled:
                    break;
                case SpriteDrawMode.Simple:
                    var bounds = spriteRenderer.sprite.bounds;
                    return new(bounds.size.x);
            }

            return null;
        }

        public override Optional<float> IntrinsicHeight(View2D view)
        {
            var spriteRenderer = SpriteRendererFor(view);
            if (null == spriteRenderer) { return null; }

            var result = IntrinsicHeightFor(spriteRenderer);
            if (null != result)
            {
                return result;
            }

            return null;
        }

        /// <summary>
        /// Non-tiled sprites have their own intrinsic height
        /// </summary>
        public Optional<float> IntrinsicHeightFor(SpriteRenderer spriteRenderer)
        {
            switch (spriteRenderer.drawMode)
            {
                case SpriteDrawMode.Sliced:
                case SpriteDrawMode.Tiled:
                    break;
                case SpriteDrawMode.Simple:
                    var bounds = spriteRenderer.sprite.bounds;
                    return new(bounds.size.y);
            }

            return null;
        }

        /// <summary>
        /// For tiled sprites, the sprite size is the view's frame size
        /// </summary>
        protected virtual Optional<Vector2> SpriteRendererSize(View2D view, SpriteRenderer spriteRenderer)
        {
            switch (spriteRenderer.drawMode)
            {
                case SpriteDrawMode.Sliced:
                case SpriteDrawMode.Tiled:
                    return new(view.Frame.size);
                case SpriteDrawMode.Simple:
                    break;
            }

            return null;
        }
    }
}
