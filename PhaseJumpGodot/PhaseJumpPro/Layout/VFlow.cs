using System;
using Godot;

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
    public partial class VFlow : SomeLayout2D
    {
        [Export]
        public float spacing = 1.0f;

        public override Vector3 Size()
        {
            return new Vector3(0, spacing * (GetChildCount() - 1), 0);
        }

        public override void ApplyLayout()
        {
            var firstPos = (Size().Y / 2) * Vector2.Up.Y;
            var position = firstPos;

            foreach (var child in GetChildren())
            {
                var transformTool = new TransformTool(child);
                transformTool.LocalPosition = new Vector3(0, position, transformTool.LocalPosition.Z);
                position += spacing * Vector2.Down.Y;
            }
        }

#if UNITY_EDITOR
        [CustomEditor(typeof(VFlow))]
        public new class Editor : SomeLayout.Editor
        {
        }
#endif
    }
}
