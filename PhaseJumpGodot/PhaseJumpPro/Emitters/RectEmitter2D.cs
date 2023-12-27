using System.Collections;
using System.Collections.Generic;
using Godot;

/*
 * RATING: 5 stars
 * Simple spawner
 * CODE REVIEW: 12/31/22
 */
namespace PJ
{
    /// <summary>
    /// Spawns objects within a rectangle onscreen
    /// </summary>
    public partial class RectEmitter2D : Emitter
    {
        public Vector2 worldSize = new(1.0f, 1.0f);

        protected override Vector3 SpawnPositionFor(Node gameObject)
        {
            float halfWidth = worldSize.X / 2.0f;
            float halfHeight = worldSize.Y / 2.0f;

            float distanceX = new EngineRandom().Delta(halfWidth);
            float distanceY = new EngineRandom().Delta(halfHeight);

            var result = new Vector3(distanceX, distanceY, 0);

            return result;
        }
    }
}
