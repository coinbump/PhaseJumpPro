using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

namespace PJ
{
	public enum DirectionState
	{
		// Core directions:
		Right, Left, Up, Down

		// 8-way directions:
		,UpLeft, UpRight, DownLeft, DownRight, None
	}

	// FUTURE: support 8-way directions if needed.
	public enum DirectionScheme
	{
		FourWay, EightWay
	}

	/// <summary>
	/// Handles states, behaviors for TopDown character (WORK IN PROGRESS)
	/// </summary>
	public class TopDown2D	// Do not subclass MonoBehaviour (so we can use new)
	{
		public GenericStateMachine<DirectionState> directionState = new GenericStateMachine<DirectionState>();

		public float moveSpeed; // Applied to move direction

		// Start is called before the first frame update
		void Start()
		{
		}

		// Update is called once per frame
		void Update()
		{
		}

		public Vector2 UpdateVelocity()
		{
			Vector2 velocity = new Vector2(0, 0);

			switch (directionState.State)
			{
				case PJ.DirectionState.Left:
					velocity = new Vector2(moveSpeed, 0)*Vector2.left;
					break;
				case PJ.DirectionState.Right:
					velocity = new Vector2(moveSpeed, 0)*Vector2.right;
					break;
				case PJ.DirectionState.Up:
					velocity = new Vector2(0, moveSpeed)*Vector2.up;
					break;
				case PJ.DirectionState.Down:
					velocity = new Vector2(0, moveSpeed)*Vector2.down;
					break;
				// FUTURE: support 8 way movement if needed.
			}

			return velocity;
		}

	}
}
