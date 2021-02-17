using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars. Simple spawner
 * CODE REVIEW: 2.15.21
 */
namespace PJ
{
	public class RectSpawner2D : Spawner
	{
		[Tooltip("Spawn inside width from center")]
		public float width;

		[Tooltip("Spawn inside height from center")]
		public float height;

#if UNITY_EDITOR
		protected override void RenderGizmos(EditorUtils.RenderState renderState)
		{
			EditorUtils.RenderRectGizmo(transform.position, width, height, renderState);
		}
#endif

		protected override Vector3 GetSpawnPosition()
		{
			Vector3 result = base.GetSpawnPosition();

			float halfWidth = width / 2.0f;
			float halfHeight = height / 2.0f;

			float distanceX = RandomUtils.Delta(halfWidth);
			float distanceY = RandomUtils.Delta(halfHeight);
			
			result += new Vector3(distanceX, distanceY, 0);

			return result;
		}
	}
}
