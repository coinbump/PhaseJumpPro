using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 11/14/22
 */
namespace PJ
{
    /// <summary>
    /// Testable utility class for View2D coordinate conversions
    /// </summary>
    [Serializable]
    public partial class View2D
    {
        public Vector3 WorldToViewPosition(Vector3 worldPosition)
        {
            var localPosition = transform.InverseTransformPoint(worldPosition);
            return LocalToViewPosition(localPosition);
        }

        public Vector3 LocalToViewPosition(Vector3 localPosition)
        {
            var viewX = localPosition.x + frame.size.x / 2.0f;
            var viewY = frame.size.y / 2.0f - localPosition.y;

            return new Vector3(viewX, viewY, 0);
        } // TESTED

        public Vector3 TopLeftWorldPosition(Vector3 transformWorldPosition)
        {
            var topLeft = new Vector3(
                transformWorldPosition.x - WorldSize.x / 2.0f,
                transformWorldPosition.y + WorldSize.y / 2.0f * Vector2.up.y,
                transformWorldPosition.z
            );
            return topLeft;
        } // TESTED

        public Vector3 TopLeftLocalPositionIn(Bounds2D parentFrame, Vector3 transformLocalPosition)
        {
            var topLeft = new Vector3(
                -parentFrame.size.x / 2.0f + frame.origin.x,
                parentFrame.size.y / 2.0f * Vector2.up.y + frame.origin.y * Vector2.down.y,
                0
            );
            return topLeft;
        } // TESTED

        public Vector3 LocalPositionIn(Bounds2D parentFrame, Vector3 transformLocalPosition)
        {
            var topLeft = TopLeftLocalPositionIn(parentFrame, transformLocalPosition);
            var result = new Vector3(
                topLeft.x + frame.size.x / 2.0f,
                topLeft.y + (frame.size.y / 2.0f) * Vector2.down.y,
                0
            );

            return result;
        } // TESTED

        public bool IsViewPositionInside(Vector3 viewPosition)
        {
            if (viewPosition.x < 0 || viewPosition.y < 0) { return false; }
            if (viewPosition.x > WorldSize.x || viewPosition.y > WorldSize.y) { return false; }

            return true;
        } // TESTED

        public Optional<float> FixedWidth => tags.Value<float>("width.fixed");
        public Optional<float> FixedHeight => tags.Value<float>("height.fixed");
        public Optional<float> MinWidth => tags.Value<float>("width.min");
        public Optional<float> MinHeight => tags.Value<float>("height.min");
        public Optional<float> MaxWidth => tags.Value<float>("width.max");
        public Optional<float> MaxHeight => tags.Value<float>("height.max");
    }
}
