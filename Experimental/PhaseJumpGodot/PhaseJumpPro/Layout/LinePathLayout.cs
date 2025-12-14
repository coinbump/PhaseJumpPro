using Godot;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/27/22
 */
namespace PJ
{
    /// <summary>
    /// Distribute objects along a circle path
    /// </summary>
    public partial class LinePathLayout : SomePathLayout
    {
        [Export]
        public Vector3 start;

        [Export]
        public Vector3 end;

        protected override SomePath BuildPath()
        {
            return new LinePath(start, end);
        }
    }
}
