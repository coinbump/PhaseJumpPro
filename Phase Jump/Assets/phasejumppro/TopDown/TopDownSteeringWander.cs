using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;
using System;

namespace PJ
{
	public class TopDownSteeringWander : TopDownSteering
	{
		public float wanderTime = 2.0f; // How long does it wander in 1 direction
		public float wanderVary = 0.5f; // Variance in wander time (in seconds)

		public GameObject target;   // Tends to wander towards target
		public float targetFactor = 2.0f;	// Weighted adjustment towards target

		private float currentWanderTime = 0.0f;

		protected void ResetTimer()
		{
			bool isVaryForward = UnityEngine.Random.value < 0.5f;
			float factor = isVaryForward ? 1.0f : -1.0f;
			currentWanderTime = wanderTime + (UnityEngine.Random.value * wanderVary * factor);
		}

		protected override void Update()
		{
			currentWanderTime -= Time.deltaTime;
			if (currentWanderTime <= 0)
			{
				ChangeDirection();
				ResetTimer();
			}

			base.Update();
		}

		public void ChangeDirection()
		{
			TopDownNode2D targetNode = GetComponent<TopDownNode2D>();
			if (null == targetNode) { return; }
			if (null == targetNode.topDown) { return; }

			WeightedRandom random = new WeightedRandom();
			Weight leftWeight = new Weight(1.0f, "left");
			Weight upWeight = new Weight(1.0f, "up");
			Weight rightWeight = new Weight(1.0f, "right");
			Weight downWeight = new Weight(1.0f, "down");

			random.Add(leftWeight);
			random.Add(upWeight);
			random.Add(rightWeight);
			random.Add(downWeight);

			// It has to change direction.
			switch (targetNode.topDown.directionState.State)
			{
				case TopDown2D.DirectionState.Left:
					random.Remove(leftWeight);
					break;
				case TopDown2D.DirectionState.Right:
					random.Remove(rightWeight);
					break;
				case TopDown2D.DirectionState.Up:
					random.Remove(upWeight);
					break;
				case TopDown2D.DirectionState.Down:
					random.Remove(downWeight);
					break;
			}

			// Keep for reference:
			//Array values = Enum.GetValues(typeof(TopDown2D.DirectionState));
			//System.Random sysRandom = new System.Random();
			//TopDown2D.DirectionState newDirection = (TopDown2D.DirectionState)values.GetValue(sysRandom.Next(values.Length));

			TopDown2D.DirectionState newDirection = targetNode.topDown.directionState.State;

			Bounds bounds = GetComponent<Renderer>().bounds;

			if (null != target)
			{
				Bounds targetBounds = target.GetComponent<Renderer>().bounds;
				if (targetBounds.center.x > bounds.center.x)
				{
					rightWeight.DefaultValue *= targetFactor;
				}
				else if (targetBounds.center.x < bounds.center.x)
				{
					leftWeight.DefaultValue *= targetFactor;
				}

				if (targetBounds.center.y > bounds.center.y)
				{
					upWeight.DefaultValue *= targetFactor;
				}
				else if (targetBounds.center.y < bounds.center.y)
				{
					downWeight.DefaultValue *= targetFactor;
				}
			}

			Weight weight = random.ChooseRandom();
			if (weight == leftWeight)
			{
				newDirection = TopDown2D.DirectionState.Left;
			}
			else if (weight == upWeight)
			{
				newDirection = TopDown2D.DirectionState.Up;
			}
			else if (weight == downWeight)
			{
				newDirection = TopDown2D.DirectionState.Down;
			}
			else if (weight == rightWeight)
			{
				newDirection = TopDown2D.DirectionState.Right;
			}

			targetNode.topDown.directionState.State = newDirection;
		}

		private void OnCollisionEnter2D(Collision2D collision)
		{
			TopDownNode2D targetNode = GetComponent<TopDownNode2D>();
			if (null == targetNode) { return; }
			if (null == targetNode.topDown) { return; }

			if (DoesCollisionAffectSteering(collision))
			{
				targetNode.topDown.directionState.State = targetNode.topDown.directionState.State.Flip();
			}
		}

		protected override void Start()
		{
			base.Start();

			ResetTimer();
		}
	}
}
