using UnityEngine;
using System;

/*
 * RATING: 4 stars
 * Tested and works. Could use collider pinning for rotated objects with non-tight meshes
 * CODE REVIEW: 4/14/22
 */
namespace PJ
{
    /// <summary>
    /// Limits a sprite to stay within a specified rect bounds onscreen
    /// LIMITATIONS: doesn't always get to the rect edge with a rotated sprite (Depends on sprite mesh)
    /// </summary>
    public abstract class SomeRectTransformLimiter2D : SomeTransformLimiter2D
    {
        public abstract Vector2 Size { get; }

        public override void Scan(GameObject target)
        {
            var spriteRenderer = target.GetComponent<SpriteRenderer>();
            if (null == spriteRenderer)
            {
                Debug.Log("Limiter2D requires sprite");
                return;
            }

            var size = Size;
            var limitBounds = new Bounds(new Vector2(transform.position.x, transform.position.y), new Vector3(size.x, size.y, 0));
            var targetBounds = spriteRenderer.bounds;

            // FUTURE: support collider type limits if needed (currently untested)
            //switch (collideType)
            //{
            //    case CollideType.Collider:
            //        var collider = target.GetComponent<Collider2D>();
            //        if (null != collider)
            //        {
            //            targetBounds = collider.bounds;
            //            break;
            //        }
            //        break;
            //    default:
            //        break;
            //}

            switch (targetLimitType)
            {
                case TargetLimitType.Center:
                    {
                        var position = new Vector2(targetBounds.center.x, targetBounds.center.y);
                        //Debug.Log("Limit Bounds: " + limitBounds.ToString() + " targetPosition: " + position.ToString());

                        var x = Mathf.Min(Mathf.Max(position.x, limitBounds.min.x), limitBounds.max.x);
                        var y = Mathf.Min(Mathf.Max(position.y, limitBounds.min.y), limitBounds.max.y);

                        var newPosition = new Vector3(x, y, target.transform.position.z);
                        UpdatePosition(target, newPosition);
                        break;
                    }
                case TargetLimitType.Bounds:
                    {
                        var position = new Vector2(targetBounds.center.x, targetBounds.center.y);
                        //Debug.Log("Limit Bounds: " + limitBounds.ToString() + " targetPosition: " + position.ToString());

                        var x = Mathf.Min(Mathf.Max(position.x, limitBounds.min.x + targetBounds.size.x / 2.0f), limitBounds.max.x - targetBounds.size.x / 2.0f);
                        var y = Mathf.Min(Mathf.Max(position.y, limitBounds.min.y + targetBounds.size.y / 2.0f), limitBounds.max.y - targetBounds.size.y / 2.0f);

                        var newPosition = new Vector3(x, y, target.transform.position.z);
                        UpdatePosition(target, newPosition);
                        break;
                    }
            }
        }

#if UNITY_EDITOR
        protected override void RenderGizmos(EditorUtils.RenderState renderState)
        {
            EditorUtils.RenderRect(transform.position, Size.x, Size.y, renderState);
        }
#endif
    }

    public class RectTransformLimiter2D : SomeRectTransformLimiter2D
    {
        public Vector2 size = new Vector2(3.0f, 3.0f);

        public override Vector2 Size => size;
    }
}
