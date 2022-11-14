using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/27/22
 */
namespace PJ
{
	/// <summary>
	/// A stage entity exists on a stage, which handles common animations for presenting
	/// and dismissing objects.
    /// Example: a visual novel where the characters slide in and out as they speak
	/// </summary>
	public class StageEntity : MonoBehaviour
	{
		public enum StagePosition
		{
			Left, Right, Top, Bottom
		}
		
		/// <summary>
        /// If true, this object is visible and presented
        /// If false, this object has been moved out of the way (usually with a fade animation)
        /// </summary>
		public bool isOnStage = false;

		/// <summary>
		/// Defines the default enter/exit animation for this object.
		/// Stage left means this slides to the left to exit and slides right to appear.
        /// Ignored if you provide a path
		/// </summary>
		public StagePosition stagePosition = StagePosition.Left;

		/// <summary>
        /// Time for enter/exit animation
        /// </summary>
		public float enterExitTime = 0.3f;

		/// <summary>
        /// Link to a path layout to override default enter/exit path
        /// </summary>
		public SomePathLayout enterExitPathLayout;

		/// <summary>
        /// Offset for the default enter/exit path.
        /// Ignored if you provide a path
        /// </summary>
		public float enterExitOffset = 2.0f;

		protected Vector3 referenceLocalPosition;

		/// <summary>
        /// Handles valve state for enter/exit animations
        /// </summary>
		protected class SlideValve : Valve
		{
			WeakReference<StageEntity> owner;

			public SlideValve(StageEntity owner, float enterExitTime)
			{
				// FUTURE: allow these values to be customized
				this.owner = new WeakReference<StageEntity>(owner);
				turnOffTimer = new TransformTimer(new InterpolateOutSquared(), enterExitTime);
				turnOnTimer = new TransformTimer(new InterpolateOutSquared(), enterExitTime);
			}

			protected override void OnValveStateChange()
			{
				//Debug.Log("Stage OnValveStateChange: " + valveState.ToString());
				
				if (!owner.TryGetTarget(out StageEntity entity)) { return; }
				entity.OnValveStateChange();
			}
		}

		protected Valve slideValve;
		protected SomeNode node;

		public void EnterStage()
		{
			if (null == slideValve) { return; }
			slideValve.TurnOn(InputEffect.Timed);
		}

		public void ExitStage()
		{
			if (null == slideValve) { return; }
			slideValve.TurnOff(InputEffect.Timed);
		}

		protected override void Awake()
		{
			node = GetComponent<SomeNode>();

			referenceLocalPosition = transform.localPosition;
			slideValve = new SlideValve(this, enterExitTime);

			// Create an enter/exit path if needed
			SetupEnterExitPathLayout();

			// Give the slide path a position
			if (enterExitPathLayout.positions.Count < 1)
            {
				enterExitPathLayout.positions.Add(slideValve.ValveState);
			}
		}

		/// <summary>
        /// Setup the path that will be used for enter/exit animations
        /// </summary>
		protected virtual void SetupEnterExitPathLayout()
        {
			if (null != enterExitPathLayout) { return; }

			var layoutParent = new GameObject();
			layoutParent.transform.parent = transform.parent;
			layoutParent.name = "Layout parent for: " + name;
			layoutParent.transform.position = transform.position;
			transform.parent = layoutParent.transform;

			var layout = layoutParent.AddComponent<LinePathLayout>();
			layout.orientToPath = false;
			enterExitPathLayout = layout;

			switch (stagePosition)
            {
				case StagePosition.Left:
					layout.start = new Vector3(-enterExitOffset, 0, 0);
					break;
				case StagePosition.Right:
					layout.start = new Vector3(enterExitOffset, 0, 0);
					break;
				case StagePosition.Top:
					layout.start = new Vector3(0, enterExitOffset * Vector2.up.y, 0);
					break;
				case StagePosition.Bottom:
					layout.start = new Vector3(0, enterExitOffset * Vector2.down.y, 0);
					break;
            }
		}

		protected override void Update()
        {
			slideValve.OnUpdate(new TimeSlice(Time.deltaTime));
			//Debug.Log("Stage Update: " + slideValve.ValveState.ToString());
		}

		protected override void Start()
		{
			//Debug.Log("Start Reference x" + referenceLocalPosition.x.ToString());

			if (isOnStage)
			{
				slideValve.TurnOn(InputEffect.Immediate);
			}
			OnValveStateChange();
		}

		protected virtual void OnValveStateChange()
		{
			var valveState = slideValve.ValveState;
			if (null != node)
			{
				Color color = node.multiRenderer.Color;
				color.a = valveState;
				node.multiRenderer.Color = color;
			}

			if (null == enterExitPathLayout) { return; }
			//Debug.Log("Stage Layout Valve: " + valveState.ToString());
			enterExitPathLayout.positions[0] = valveState;
			enterExitPathLayout.ApplyLayout();
		}
	}
}
