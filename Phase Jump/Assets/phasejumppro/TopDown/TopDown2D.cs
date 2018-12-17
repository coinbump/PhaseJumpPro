using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

namespace PJ
{
	/// <summary>
	/// Handles states, behaviors for TopDown character (WORK IN PROGRESS)
	/// </summary>
	public class TopDown2D	// Do not subclass MonoBehaviour (so we can use new)
	{
		public enum DirectionState
		{
			Left, Right, Up, Down, None
		}
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
				case PJ.TopDown2D.DirectionState.Left:
					velocity = new Vector2(-moveSpeed, 0);
					break;
				case PJ.TopDown2D.DirectionState.Right:
					velocity = new Vector2(moveSpeed, 0);
					break;
				case PJ.TopDown2D.DirectionState.Up:
					velocity = new Vector2(0, moveSpeed);
					break;
				case PJ.TopDown2D.DirectionState.Down:
					velocity = new Vector2(0, -moveSpeed);
					break;
			}

			return velocity;
		}

	}

	public static class TopDown2DDirectionStateExtension
	{
		public static TopDown2D.DirectionState Flip(this TopDown2D.DirectionState state)
		{
			TopDown2D.DirectionState result = state;

			switch (state)
			{
				case TopDown2D.DirectionState.Left:
					result = TopDown2D.DirectionState.Right;
					break;
				case TopDown2D.DirectionState.Right:
					result = TopDown2D.DirectionState.Left;
					break;
				case TopDown2D.DirectionState.Up:
					result = TopDown2D.DirectionState.Down;
					break;
				case TopDown2D.DirectionState.Down:
					result = TopDown2D.DirectionState.Up;
					break;
			}

			return result;
		}

		public static TopDown2D.DirectionState RotateLeft(this TopDown2D.DirectionState state)
		{
			TopDown2D.DirectionState result = state;

			switch (state)
			{
				case TopDown2D.DirectionState.Left:
					result = TopDown2D.DirectionState.Down;
					break;
				case TopDown2D.DirectionState.Right:
					result = TopDown2D.DirectionState.Up;
					break;
				case TopDown2D.DirectionState.Up:
					result = TopDown2D.DirectionState.Left;
					break;
				case TopDown2D.DirectionState.Down:
					result = TopDown2D.DirectionState.Right;
					break;
			}

			return result;
		}

		public static TopDown2D.DirectionState RotateRight(this TopDown2D.DirectionState state)
		{
			TopDown2D.DirectionState result = state;

			switch (state)
			{
				case TopDown2D.DirectionState.Left:
					result = TopDown2D.DirectionState.Up;
					break;
				case TopDown2D.DirectionState.Right:
					result = TopDown2D.DirectionState.Down;
					break;
				case TopDown2D.DirectionState.Up:
					result = TopDown2D.DirectionState.Right;
					break;
				case TopDown2D.DirectionState.Down:
					result = TopDown2D.DirectionState.Left;
					break;
			}

			return result;
		}
	}
}
