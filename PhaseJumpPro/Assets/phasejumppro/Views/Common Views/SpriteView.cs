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
                                break;
                            case SpriteDrawMode.Simple:
                                var bounds = spriteRenderer.sprite.bounds;
                                return new(bounds.size.x * scale.x);
                                break;
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
                                break;
                            case SpriteDrawMode.Simple:
                                var bounds = spriteRenderer.sprite.bounds;
                                return new(bounds.size.y * scale.y);
                                break;
                        }
                    }
                    return base.IntrinsicHeight;
                }
                set => base.IntrinsicHeight = value;
            }
        }
    }
}
