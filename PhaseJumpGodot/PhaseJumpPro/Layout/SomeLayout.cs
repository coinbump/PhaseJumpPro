using Godot;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple abstract class
 * CODE REVIEW: 12/27/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Defines the layout of child game objects
    /// (different than Unity's Layout Group for UI, which requires a Canvas)
    /// </summary>
    public abstract partial class SomeLayout : PJ.WorldNode
    {
        /// <summary>
        /// If true, layout will be updated automatically.
        /// If false, you must update layout manually
        /// </summary>
        [Export]
        public bool autoApply = true;

        /// <summary>
        /// Return the bounds-size of the layout
        /// </summary>
        /// <returns></returns>
        public virtual Vector3 Size()
        {
            return Vector3.Zero;
        }

        /// <summary>
        /// Arrange child objects according to the layout
        /// </summary>
        public abstract void ApplyLayout();

        public override void Start()
        {
            base.Start();

            if (autoApply)
            {
                ApplyLayout();
            }
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            if (autoApply)
            {
                ApplyLayout();
            }
        }

        public virtual Vector3 LayoutPositionAt(int index)
        {
            return Vector3.Zero;
        }

        // FUTURE: support "Apply Layout" button from editor
    }
}
