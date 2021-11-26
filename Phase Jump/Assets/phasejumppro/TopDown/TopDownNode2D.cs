using UnityEngine;
using System.Collections;
using PJ;

namespace PJ
{
	/// <summary>
	/// Node with TopDown and core compnents.
	/// </summary>
	public class TopDownNode2D : GoNode2D
	{
		public class TopDown2D : PJ.TopDown2D
		{
			TopDownNode2D owner;

			public TopDown2D(TopDownNode2D owner)
			{
				this.owner = owner;
				directionState.AddListener(owner.core.listener);
			}
		}
		[HideInInspector]
		public TopDown2D topDown;

		[SerializeField]
		private DirectionState direction = DirectionState.Right;	// Only used when character starts.

		/// <summary>
		/// Handles state machine.
		/// </summary>
		new protected class Core : PJ.Core
		{
			TopDownNode2D owner;

			public Core(TopDownNode2D owner)
			{
				this.owner = owner;
			}

			protected override void EvtStateChanged(SomeStateMachine state)
			{
				base.EvtStateChanged(state);

				if (state == owner.topDown.directionState)
				{
					owner.UpdateVelocity();
				}
			}
		}
		protected Core core;

		protected virtual void UpdateVelocity()
		{
			if (null != topDown)
			{
				velocity = topDown.UpdateVelocity();
			}
		}

		protected override void Start()
		{
			base.Start();

			if (null != topDown)
			{
				topDown.directionState.State = direction;
			}
		}
	}
}
