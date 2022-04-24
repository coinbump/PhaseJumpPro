using System;

namespace PJ
{
	public class MovePath : SomeMovePath
	{
		/// <summary>
        /// Path model for this move path
        /// </summary>
		public SomePath path;

		protected override void SnapPathMover(PathMover pathMover, float progress, bool force)
		{
			if (null == path) { return; }
			var position = path.PositionAt(progress);
			MovePathMoverToLocalPosition(pathMover, position, force);
		}
	}
}
