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
    public partial class HFlow : SomeLayout2D
    {
        [Export]
        public float spacing = 1.0f;

        public override Vector3 Size()
        {
            return new Vector3(spacing * (GetChildCount() - 1), 0, 0);
        }

        public override void ApplyLayout()
        {
            var firstPos = -Size().X / 2;
            var position = firstPos;

            foreach (var child in GetChildren())
            {
                var transformTool = new TransformTool(child);
                transformTool.SetLocalPosition(new Vector3(position, 0, transformTool.LocalPosition.Z));

                position += spacing;
            }
        }
    }
}
