using UnityEngine;
using System.Collections;
using PJ;

namespace PJ
{
	public class TopDownSteeringRotate : TopDownSteering
	{
		public enum RotateType
		{
			Right, Left, Random
		}
		public RotateType type;

		private void OnCollisionEnter2D(Collision2D collision)
		{
			TopDownNode2D target = GetComponent<TopDownNode2D>();
			if (null == target) { return; }

			if (!DoesCollisionAffectSteering(collision)) { return; }

			RotateType thisType = type;
			if (RotateType.Random == thisType)
			{
				thisType = Random.Range(0.0f, 1.0f) > 0.5f ? RotateType.Left : RotateType.Right;
			}

			switch (thisType)
			{
				case RotateType.Left:
					target.topDown.directionState.State = target.topDown.directionState.State.RotateLeft(DirectionScheme.FourWay);
					break;
				case RotateType.Right:
					target.topDown.directionState.State = target.topDown.directionState.State.RotateRight(DirectionScheme.FourWay);
					break;
			}
		}
	}
}
