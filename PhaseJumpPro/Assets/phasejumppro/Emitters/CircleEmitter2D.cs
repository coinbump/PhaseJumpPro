using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple spawner
 * CODE REVIEW: 12/31/22
 */
namespace PJ
{
    public class CircleEmitter2D : Emitter
    {
        [Tooltip("Spawn within radius")]
        public float radius;
        protected override Vector3 SpawnPositionFor(GameObject gameObject)
        {
            float distance = RandomUtils.VaryFloat(0, 1.0f) * radius;
            float angle = spawnTable.random.Value * 360.0f;

            Vector2 offset = (Vector2)Angle.DegreesAngle(angle) * distance;
            var result = new Vector3(offset.x, offset.y, 0);

            return result;
        }

#if UNITY_EDITOR
        protected override void RenderGizmos(EditorUtils.RenderState renderState)
        {
            EditorUtils.DrawCircle(transform.position, radius, renderState);
        }
#endif
    }
}
