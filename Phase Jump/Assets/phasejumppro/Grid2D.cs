using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// TODO: Work in progress
namespace PJ
{
	public class GridLoc : MonoBehaviour
	{
		public Vector3Int value;
		public Vector3 offset;
	}

	/// <summary>
	/// Unlike Unity's Grid, ours automatically places objects in the correct position
	/// when the grid is resized.
	/// </summary>
	public class Grid2D : MonoBehaviour
	{
		public Vector2Int size = new Vector2Int(1, 1);
		public Vector2 cellSize = new Vector2Int(1, 1);
		public Vector2 cellGap;

		void Start()
		{

		}

		void Update()
		{

		}

		Rect CellBounds(GridLoc loc)
		{
			var result = new Rect();

			result.xMin = loc.value.x * cellSize.x;
			if (loc.value.x > 0)
			{
				result.xMin += (loc.value.x - 1) * cellGap.x;
			}
			result.xMax = loc.value.y * cellSize.y;
			if (loc.value.y > 0)
			{
				result.yMin += (loc.value.y - 1) * cellGap.y;
			}

			result.size = cellSize;

			// TODO: are we using Cartesian or reading coordinates?

			return result;
		}

		private GridLoc GridLocFor(GameObject go)
		{
			var gridLoc = go.GetComponent<GridLoc>();
			if (null == gridLoc)
			{
				gridLoc = go.AddComponent<GridLoc>();

				// TODO: find the nearest empty grid loc and assign it to the object <- what if there is no empty cell?
			}
			return gridLoc;
		}

		/// <summary>
		/// For each child object, make sure it has a grid loc, or create one.
		/// </summary>
		private void ForceChildLocs()
		{
			foreach (Transform child in transform)
			{
				var go = child.gameObject;
				var gridLoc = GridLocFor(go);
			}
		}

		void OnTransformChildrenChanged()
		{
			ResizeChildren();
		}

		private void ResizeChildren()
		{
			foreach (Transform child in transform)
			{
				var go = child.gameObject;
				var loc = GridLocFor(go);
				var bounds = CellBounds(loc);
				go.transform.position = bounds.center;	// TODO: support offset
			}
		}

		private void OnValidate()
		{
			ResizeChildren();
		}
	}
}
