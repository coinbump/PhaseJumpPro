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
	/// <summary>
    /// Spawns objects within a rectangle onscreen
    /// </summary>
	public class RectSpawner2D : Spawner
	{
		public Vector2 worldSize = new Vector2(1.0f, 1.0f);

#if UNITY_EDITOR
		protected override void RenderGizmos(EditorUtils.RenderState renderState)
		{
			EditorUtils.DrawRect(transform.position, worldSize.x, worldSize.y, renderState);
		}
#endif

		protected override Vector3 SpawnPositionFor(GameObject gameObject)
		{
			float halfWidth = worldSize.x / 2.0f;
			float halfHeight = worldSize.y / 2.0f;

			float distanceX = RandomUtils.Delta(halfWidth);
			float distanceY = RandomUtils.Delta(halfHeight);
			
			var result = new Vector3(distanceX, distanceY, 0);

			return result;
		}
	}
}
