using System;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// View that fits to the size of its sprite content
    /// </summary>
    public class SpriteView : View2D
    {
        public override Optional<float> IntrinsicWidth {
            get
            {
                if (TryGetComponent(out SpriteRenderer spriteRenderer))
                {
                    var scale = spriteRenderer.gameObject.transform.localScale;

                    switch (spriteRenderer.drawMode)
                    {
                        case SpriteDrawMode.Sliced:
                        case SpriteDrawMode.Tiled:
                            return base.IntrinsicWidth;
                        case SpriteDrawMode.Simple:
                            var bounds = spriteRenderer.sprite.bounds;
                            return new(bounds.size.x * scale.x);
                    }
                }
                return base.IntrinsicWidth;
            }
            set => base.IntrinsicWidth = value;
        }

        public override Optional<float> IntrinsicHeight {
            get
            {
                if (TryGetComponent(out SpriteRenderer spriteRenderer))
                {
                    var scale = spriteRenderer.gameObject.transform.localScale;

                    switch (spriteRenderer.drawMode)
                    {
                        case SpriteDrawMode.Sliced:
                        case SpriteDrawMode.Tiled:
                            return base.IntrinsicHeight;
                        case SpriteDrawMode.Simple:
                            var bounds = spriteRenderer.sprite.bounds;
                            return new(bounds.size.y * scale.y);
                    }
                }
                return base.IntrinsicHeight;
            }
            set => base.IntrinsicHeight = value;
        }

        protected override void _ApplyLayout(Vector2 layoutSize)
        {
            base._ApplyLayout(layoutSize);

            UpdateSpriteRenderer();
        }

        protected override void OnFrameChange()
        {
            base.OnFrameChange();
            UpdateSpriteRenderer();
        }

        protected void UpdateSpriteRenderer()
        {
            if (TryGetComponent(out SpriteRenderer spriteRenderer))
            {
                switch (spriteRenderer.drawMode)
                {
                    case SpriteDrawMode.Sliced:
                    case SpriteDrawMode.Tiled:
                        spriteRenderer.size = Frame.size;
                        break;
                    case SpriteDrawMode.Simple:
                        break;
                }
            }
        }
    }
}
