using UnityEngine;
using System.Collections;
using PJ;
using System.Collections.Generic;

namespace PJ
{
	public class TopDownSteeringFlip : TopDownSteering
	{
		private void OnCollisionEnter2D(Collision2D collision)
		{
			TopDownNode2D target = GetComponent<TopDownNode2D>();
			if (null == target) { return; }
			if (null == target.topDown) { return; }

			if (DoesCollisionAffectSteering(collision)) {
				target.topDown.directionState.State = target.topDown.directionState.State.Flip();
			}
		}
	}
}
