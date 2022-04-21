using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
	/// <summary>
	/// Spawns objects within a rectangle onscreen
	/// </summary>
	public class GridSpawner2D : Spawner
	{
		[Space(10)]
		[Header("GridSpawner2D Properties")]

		public Vector2 worldSize = new Vector2(1.0f, 1.0f);
		public Vector2Int gridSize = new Vector2Int(3, 3);

		/// <summary>
        /// Offset of spawned object from cell center
        /// </summary>
		public Vector2 spawnOffset = Vector2.zero;

		/// <summary>
        /// Random variation of spawned object from cell center
        /// </summary>
		public Vector2 varyOffset;

		/// <summary>
        /// The max # of spawns that can occur per grid cell
        /// </summary>
		public int maxPerCell = 1;

		protected MatrixInt occupiedMatrix;

		public bool IsCellFull(Vector2Int location)
        {
			return occupiedMatrix.CellAt(location) >= maxPerCell;
        }

		public Vector2 CellSize
		{
			get
			{
				return new Vector2(worldSize.x / (float)gridSize.x, worldSize.y / (float)gridSize.y);
			}
		}

		protected float ColumnLineX(int column)
		{
			return -worldSize.x / 2.0f + column * CellSize.x;
		}

		protected float RowLineY(int row)
        {
			return (worldSize.y / 2.0f) * Vector2.up.y + row * CellSize.y * Vector2.down.y;
        }

		public override void Awake()
        {
			base.Awake();

			// For grids we need to keep track of occupied cells, so the objects must be children of the spawner
			spawnParent = gameObject;

			occupiedMatrix = new MatrixInt(gridSize);
		}

        protected override void OnSpawn(GameObject gameObject)
        {
            base.OnSpawn(gameObject);

			var node = gameObject.GetComponent<SomeNode>();
			if (null == node) { return; }

			if (node.tags.TryGetValue("location", out object value))
            {
				if (value is Vector2Int)
                {
					var valueVector = (Vector2Int)value;
					occupiedMatrix.SetCell(valueVector, occupiedMatrix.CellAt(valueVector) + 1);
                }
			}
        }

		public override bool CanSpawn()
        {
			var freeCells = UpdateOccupiedMatrix();
			return freeCells.Count > 0;
		}

        protected override Vector3 SpawnPositionFor(GameObject gameObject)
		{
			var freeCells = UpdateOccupiedMatrix();
			if (freeCells.Count <= 0) { return Vector3.zero; }

			int cellIndex = Mathf.RoundToInt(random.Value * (float)(freeCells.Count - 1));
			var cellLocation = freeCells[cellIndex];

			var cellCenterX = -worldSize.x / 2.0f + CellSize.x / 2.0f + cellLocation.x * CellSize.x;
			var cellCenterY = (worldSize.y / 2.0f) * Vector2.up.y + (CellSize.y / 2.0f + cellLocation.y * CellSize.y) * Vector2.down.y;

			var node = gameObject.GetComponent<SomeNode>();
			if (null == node) {
				Debug.Log("Error. Node is required for GridSpawner");
				return Vector3.zero;
			}

			node.tags.Add("location", cellLocation);

			var result = new Vector3(cellCenterX, cellCenterY, 0);
			result.x += RandomUtils.VaryFloat(spawnOffset.x, varyOffset.x);
			result.y += RandomUtils.VaryFloat(spawnOffset.y, varyOffset.y);

			return result;
        }

		protected List<Vector2Int> UpdateOccupiedMatrix()
        {
			for (int x = 0; x < occupiedMatrix.Width; x++)
            {
				for (int y = 0; y < occupiedMatrix.Height; y++)
                {
					occupiedMatrix.SetCell(new Vector2Int(x, y), 0);
				}
            }

			// This only works if the spawned objects are children of the spawner
			foreach (Transform childTransform in transform)
            {
				var node = childTransform.gameObject.GetComponent<SomeNode>();
				if (null == node) { continue; }

				if (node.tags.TryGetValue("location", out object value))
				{
					if (value is Vector2Int)
					{
						var valueVector = (Vector2Int)value;
						var newValue = occupiedMatrix.CellAt(valueVector) + 1;
						occupiedMatrix.SetCell(valueVector, newValue);
					}
				}
			}

			var freeCells = new List<Vector2Int>();

			for (int x = 0; x < occupiedMatrix.Width; x++)
			{
				for (int y = 0; y < occupiedMatrix.Height; y++)
				{
					var location = new Vector2Int(x, y);
					if (occupiedMatrix.CellAt(location) < maxPerCell)
                    {
						freeCells.Add(location);
                    }
				}
			}

			return freeCells;
		}

#if UNITY_EDITOR
        protected override void RenderGizmos(EditorUtils.RenderState renderState)
		{
			// Render columns
			for (int x = 0; x < (gridSize.x + 1); x++)
			{
				Vector3 v1 = transform.TransformPoint(new Vector3(ColumnLineX(x), (worldSize.y / 2.0f) * Vector2.up.y, 0));
				Vector3 v2 = transform.TransformPoint(new Vector3(ColumnLineX(x), (worldSize.y / 2.0f) * Vector2.down.y, 0));

				Gizmos.DrawLine(v1, v2);
			}

			// Render rows
			for (int y = 0; y < (gridSize.y + 1); y++)
			{
				Vector3 v1 = transform.TransformPoint(new Vector3(-(worldSize.x / 2.0f), RowLineY(y)));
				Vector3 v2 = transform.TransformPoint(new Vector3((worldSize.x / 2.0f), RowLineY(y)));

				Gizmos.DrawLine(v1, v2);
			}
		}
#endif
	}
}
