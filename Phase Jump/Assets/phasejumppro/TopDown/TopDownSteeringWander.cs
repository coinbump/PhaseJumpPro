using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;
using System;

namespace PJ
{
	public class TopDownSteeringWander : TopDownSteering
	{
		public float value = 2.0f; // Time/distance to wander
		public float vary; // Variance in wander value

		public enum WanderType
		{
			Time, Distance
		}
		public WanderType type;

		public float restTime;  // Time to rest before changing directions
		public float restVary;  // Variance in rest value

		public GameObject target;   // Tends to wander towards target
		public float targetFactor = 2.0f;	// Weighted adjustment towards target

		private float currentValue = 0.0f;

		protected Vector3 startPosition;
		protected Timer restTimer = new Timer(AbstractTimed.Type.Persistent);
		protected DirectionState nextDirection;

		protected float VaryFloat(float value, float vary)
		{
			bool isVaryForward = UnityEngine.Random.value < 0.5f;
			float factor = isVaryForward ? 1.0f : -1.0f;
			float result = Mathf.Max(0, value + (UnityEngine.Random.value * vary * factor));

			return result;
		}

		protected void ResetTimer()
		{
			bool isVaryForward = UnityEngine.Random.value < 0.5f;
			switch (type)
			{
				case WanderType.Time:
					currentValue = VaryFloat(value, vary);
					break;
				case WanderType.Distance:
					currentValue = Mathf.Max(0, value + Mathf.Round(VaryFloat(value, vary)));
					break;
			}
		}

		protected override void Update()
		{
			TopDownNode2D targetNode = GetComponent<TopDownNode2D>();
			if (null == targetNode) { return; }
			if (null == targetNode.topDown) { return; }

			switch (type)
			{
				case WanderType.Time:
					currentValue -= Time.deltaTime;
					if (currentValue <= 0)
					{
						ChangeDirection();
					}
					break;
				case WanderType.Distance:
					Vector3 position = gameObject.transform.position;

					float distance = Mathf.Abs(Vector3.Distance(startPosition, position));
					if (distance >= currentValue)
					{
						ChangeDirection();
					}
					break;
			}

			if (restTimer.IsRunning && restTimer.duration > 0)
			{
				restTimer.EvtUpdate(new TimeSlice(Time.deltaTime));
				if (restTimer.IsFinished)
				{
					targetNode.topDown.directionState.State = nextDirection;
				}
			}
			
			base.Update();
		}

		public void ChangeDirection()
		{
			restTimer.duration = VaryFloat(restTime, restVary);
			restTimer.Reset();

			startPosition = gameObject.transform.position;

			TopDownNode2D targetNode = GetComponent<TopDownNode2D>();
			if (null == targetNode) { return; }
			if (null == targetNode.topDown) { return; }

			WeightedRandom random = new WeightedRandom();
			Weight leftWeight = new Weight(1.0f, "left");
			Weight upWeight = new Weight(1.0f, "up");
			Weight rightWeight = new Weight(1.0f, "right");
			Weight downWeight = new Weight(1.0f, "down");

			List<DirectionState> directions = new List<DirectionState>();
			directions.Add(DirectionState.Left);
			directions.Add(DirectionState.Right);
			directions.Add(DirectionState.Up);
			directions.Add(DirectionState.Down);// It has to change direction.

			RemoveBlockedDirections(directions);

			switch (targetNode.topDown.directionState.State)
			{
				case DirectionState.Left:
					directions.Remove(DirectionState.Left);
					break;
				case DirectionState.Right:
					directions.Remove(DirectionState.Right);
					break;
				case DirectionState.Up:
					directions.Remove(DirectionState.Up);
					break;
				case DirectionState.Down:
					directions.Remove(DirectionState.Down);
					break;
			}

			if (directions.Contains(DirectionState.Left))
			{
				random.Add(leftWeight);
			}
			if (directions.Contains(DirectionState.Right))
			{
				random.Add(rightWeight);
			}

			if (directions.Contains(DirectionState.Up))
			{
				random.Add(upWeight);
			}
			if (directions.Contains(DirectionState.Down))
			{
				random.Add(downWeight);
			}

			// Keep for reference:
			//Array values = Enum.GetValues(typeof(DirectionState));
			//System.Random sysRandom = new System.Random();
			//DirectionState newDirection = (DirectionState)values.GetValue(sysRandom.Next(values.Length));

			DirectionState newDirection = targetNode.topDown.directionState.State;

			Bounds bounds = GetComponent<Renderer>().bounds;

			if (null != target)
			{
				Bounds targetBounds = target.GetComponent<Renderer>().bounds;
				if (targetBounds.center.x > bounds.center.x)
				{
					rightWeight.Value *= targetFactor;
				}
				else if (targetBounds.center.x < bounds.center.x)
				{
					leftWeight.Value *= targetFactor;
				}

				if (targetBounds.center.y > bounds.center.y)
				{
					upWeight.Value *= targetFactor;
				}
				else if (targetBounds.center.y < bounds.center.y)
				{
					downWeight.Value *= targetFactor;
				}
			}

			Weight weight = random.ChooseRandom();
			if (weight == leftWeight)
			{
				newDirection = DirectionState.Left;
			}
			else if (weight == upWeight)
			{
				newDirection = DirectionState.Up;
			}
			else if (weight == downWeight)
			{
				newDirection = DirectionState.Down;
			}
			else if (weight == rightWeight)
			{
				newDirection = DirectionState.Right;
			}

			nextDirection = newDirection;
			if (restTimer.duration > 0)
			{
				targetNode.topDown.directionState.State = DirectionState.None;
			}
			else
			{
				targetNode.topDown.directionState.State = newDirection;
			}

			ResetTimer();
		}

		private void OnCollisionEnter2D(Collision2D collision)
		{
			TopDownNode2D targetNode = GetComponent<TopDownNode2D>();
			if (null == targetNode) { return; }
			if (null == targetNode.topDown) { return; }

			if (DoesCollisionAffectSteering(collision))
			{
				ChangeDirection();
			}
		}

		protected override void Start()
		{
			base.Start();

			ResetTimer();
		}
	}
}
