using System;
using UnityEngine;

namespace PJ
{
    namespace View
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
                                return new(spriteRenderer.size.x * scale.x);
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
                                return new(spriteRenderer.size.y * scale.y);
                            case SpriteDrawMode.Simple:
                                var bounds = spriteRenderer.sprite.bounds;
                                return new(bounds.size.y * scale.y);
                        }
                    }
                    return base.IntrinsicHeight;
                }
                set => base.IntrinsicHeight = value;
            }
        }
    }
}
