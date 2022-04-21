using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple spawner
 * CODE REVIEW: 4/15/22
 */
namespace PJ
{
	public class CircleSpawner2D : Spawner
	{
		[Tooltip("Spawn within radius")]
		public float radius;

#if UNITY_EDITOR
		protected override void RenderGizmos(EditorUtils.RenderState renderState)
		{
			EditorUtils.RenderCircle(transform.position, radius, renderState);
		}
#endif

		protected override Vector3 SpawnPositionFor(GameObject gameObject)
		{
			float distance = RandomUtils.VaryFloat(0, 1.0f) * radius;
			float angle = random.Value * 360.0f;

			Vector2 offset = AngleUtils.DegreeAngleToVector2(angle, 1.0f) * distance;
			var result = new Vector3(offset.x, offset.y, 0);

			return result;
		}
	}
}
