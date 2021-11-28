using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// TODO: This is UNFINISHED (Work in progress).
namespace PJ
{
	/// <summary>
	/// Unlike Unity's Grid, ours automatically places objects in the correct position
	/// when the grid is resized.
	/// Unlike GridLayoutGroup, this allows us to specify a specific cell for each child
	/// NOTE: We can't subclass UnityEngine.Grid, because it is sealed
	/// </summary>
	public class Grid2D : MonoBehaviour
	{
		private void Awake()
		{
		}

		void Start()
		{

		}

		void Update()
		{

		}

		private SnapToGridLoc2D GridLocFor(GameObject go)
		{
			var gridLoc = go.GetComponent<SnapToGridLoc2D>();
			if (null == gridLoc)
			{
				gridLoc = go.AddComponent<SnapToGridLoc2D>();

				// TODO: find the nearest empty grid loc and assign it to the object <- what if there is no empty cell?
			}
			return gridLoc;
		}

		/// <summary>
		/// For each child object, make sure it has a grid loc, or create one.
		/// </summary>
		private void AddChildLocs()
		{
			foreach (Transform child in transform)
			{
				var go = child.gameObject;
				var gridLoc = GridLocFor(go);
			}
		}

		public void ResizeChild(GameObject go)
		{
			var grid = GetComponent<Grid>();
			if (null == grid)
			{
				Debug.LogError("ERROR. Grid2D requires a Grid component.");
				return;
			}

			var loc = GridLocFor(go);
			var position = grid.GetCellCenterLocal(loc.value);
			position += loc.offset;

			go.transform.position = position;
		}

		private void ResizeChildren()
		{
			foreach (Transform child in transform)
			{
				var go = child.gameObject;
				ResizeChild(go);
			}
		}

		void OnTransformChildrenChanged()
		{
			ResizeChildren();
		}

		private void OnValidate()
		{
			ResizeChildren();
		}
	}
}
