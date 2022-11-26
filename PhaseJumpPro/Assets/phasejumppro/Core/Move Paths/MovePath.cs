using System;

namespace PJ
{
	public abstract class MovePath : SomeMovePath
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

        protected override void Awake()
        {
            base.Awake();
            path = BuildPath();
        }

        protected abstract SomePath BuildPath();

#if UNITY_EDITOR
        protected override void OnValidate()
        {
            base.OnValidate();
            path = BuildPath();
        }
#endif
    }
}
