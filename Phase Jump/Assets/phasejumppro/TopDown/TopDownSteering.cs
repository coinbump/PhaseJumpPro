using UnityEngine;
using System.Collections;
using PJ;
using System.Collections.Generic;
using UnityEngine.Tilemaps;

namespace PJ
{
	public class TopDownSteering : MonoBehaviour
	{
		public GameObject solidsTilemap;   // Prevents steering into solids

		/// <summary>
		/// Removes the blocked directions (requires a tilemap of solid objects)
		/// </summary>
		/// <returns>Directions, without the blocked directions.</returns>
		/// <param name="directions">Directions.</param>
		// TODO: add logic for gameObjects at tile position (not a tile)
		// TODO: this logic will NOT work with gameObjects larger than a single tile (need to test at the corners, not the center).
		public List<DirectionState> RemoveBlockedDirections(List<DirectionState> directions)
		{
			List<DirectionState> result = new List<DirectionState>(directions);
			if (null == solidsTilemap) { return result; }

			Tilemap tilemap = solidsTilemap.gameObject.GetComponent<Tilemap>();
			if (null == tilemap)
			{
				return result;
			}
			Vector3Int gridPos = tilemap.WorldToCell(gameObject.transform.position);

			foreach (DirectionState direction in directions)
			{
				Vector3Int destGridPos = direction.OffsetGridPos(gridPos);
				Tile tile = tilemap.GetTile(destGridPos) as Tile;
				if (null != tile)
				{
					result.Remove(direction);
				}
			}

			return result;
		}

		/*
		 * Only make a change to the steering if we're steering towards an obstacle
		 */
		 // TODO: Re-evaluate this. Use solid tilemap instead? (or both?)
		 // TODO: keep collision directions blocked until collision exit.
		 // TODO: evaluate logic with gameObjects > 1 tile in size.
		public bool DoesCollisionAffectSteering(Collision2D collision) {

			TopDownNode2D target = GetComponent<TopDownNode2D>();
			if (null == target) { return false; }
			if (null == target.topDown) { return false; }

			Collider2D myCollider = collision.otherCollider;
			Bounds myBounds = myCollider.bounds;
			Collider2D otherCollider = collision.collider;

			bool doesColliderAffectSteering = false;

			foreach (ContactPoint2D hit in collision.contacts)
			{
				// Move inward a bit in case we hit a corner.
				Vector2 max = Vector2.Lerp(myBounds.max, myBounds.center, 0.01f);
				Vector2 min = Vector2.Lerp(myBounds.min, myBounds.center, 0.01f);

				float maxX = max.x;
				float minX = min.x;
				float maxY = max.y;
				float minY = min.y;

				switch (target.topDown.directionState.State)
				{
					case DirectionState.Left:
						doesColliderAffectSteering = hit.point.x < myBounds.center.x && hit.point.y < maxY && hit.point.y > minY;
						break;
					case DirectionState.Right:
						doesColliderAffectSteering = hit.point.x > myBounds.center.x && hit.point.y < maxY && hit.point.y > minY;
						break;
					case DirectionState.Up:
						doesColliderAffectSteering = hit.point.y > myBounds.center.y && hit.point.x < maxX && hit.point.x > minX;
						break;
					case DirectionState.Down:
						doesColliderAffectSteering = hit.point.y < myBounds.center.y && hit.point.x < maxX && hit.point.x > minX;
						break;
				}

				if (doesColliderAffectSteering)
				{
					break;
				}
			}

			return doesColliderAffectSteering;
		}

		protected virtual void Start()
		{

		}

		protected virtual void Update()
		{

		}

	}

}
