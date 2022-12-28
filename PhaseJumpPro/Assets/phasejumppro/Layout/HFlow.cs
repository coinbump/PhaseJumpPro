using UnityEditor;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple flow layout
 * CODE REVIEW: 12/27/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Flow the objects with non-contextual spacing (object size doesn't matter)
    /// </summary>
    public class HFlow : SomeLayout2D
    {
        public float spacing = 1.0f;

        public override Vector3 Size()
        {
            return new Vector3(spacing * (transform.childCount - 1), 0, 0);
        }

        public override void ApplyLayout()
        {
            var firstPos = -Size().x / 2;
            var position = firstPos;

            foreach (Transform childTransform in gameObject.transform)
            {
                childTransform.localPosition = new Vector3(position, 0, 0);

                position += spacing;
            }
        }
    }
}
