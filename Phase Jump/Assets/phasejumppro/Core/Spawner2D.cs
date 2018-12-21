using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
	public class Spawner2D : Spawner
	{
		public float radius;    // Spawn within radius

#if UNITY_EDITOR
		protected override void RenderGizmos(EditorUtils.RenderState renderState)
		{
			EditorUtils.RenderCircleGizmo(transform.position, radius, renderState);
		}
#endif

		protected override Vector3 GetSpawnPosition()
		{
			Vector3 result = base.GetSpawnPosition();

			float distance = RandomUtils.VaryFloat(0, 1.0f) * radius;
			float angle = Random.value * 360.0f;

			Vector2 offset = AngleUtils.DegreeAngleToVector2(angle, 1.0f) * distance;
			result = result + new Vector3(offset.x, offset.y, 0);

			return result;
		}
	}

}
