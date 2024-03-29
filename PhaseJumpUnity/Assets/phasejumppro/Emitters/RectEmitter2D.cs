﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

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
    public class RectEmitter2D : Emitter
    {
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);

        protected override Vector3 SpawnPositionFor(GameObject gameObject)
        {
            float halfWidth = worldSize.x / 2.0f;
            float halfHeight = worldSize.y / 2.0f;

            float distanceX = new UnityRandom().Delta(halfWidth);
            float distanceY = new UnityRandom().Delta(halfHeight);

            var result = new Vector3(distanceX, distanceY, 0);

            return result;
        }

#if UNITY_EDITOR
        protected override void RenderGizmos(EditorUtils.RenderState renderState)
        {
            EditorUtils.DrawRect(transform.position, worldSize.x, worldSize.y, renderState);
        }
#endif
    }
}
