using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace PJ
{
	public class AbstractGridBoard : Core {
	}

	public class GenericGridBoard<Tile> : AbstractGridBoard where Tile : GridTile
	{
		#region Constants
		struct Events
		{
			public const string EvtAddTile = "tile_add";
			public const string EvtWillRemoveTile = "tile_will_remove";
			public const string EvtRemovedTile = "tile_removed";
		}
		#endregion

		#region Fields
		protected HashSet<Tile> tiles = new HashSet<Tile>();
		protected Vector3Int size = new Vector3Int(0, 0, 0);
		protected List<GenericGridLayer<Tile>> layers = new List<GenericGridLayer<Tile>>();
		protected bool suspendEvents;   // If true, don't broadcast events
		#endregion

		#region Properties
		public int Width => size.x;
		public int Height => size.y;
		public int Depth => size.z;
		#endregion

		#region Builders
		public bool PutTile(Tile tile, Vector3Int loc)
		{
			if (!IsValidLoc(loc)) { return false; }
			if (null == tile) { return false; }

			if (tile.size.x < 1 || tile.size.y < 1)
			{
				Debug.LogWarningFormat("ERROR. Invalid tile size {0}, {1}", tile.size.x, tile.size.y);
				return false;
			}

			Rect2Int tileBounds = GetDestTileBounds(tile, loc);
			if (IsBlocked(tileBounds, loc.z))
			{
				return false;
			}

			var layer = layers[loc.z];

			// Store all tiles here for easy iteration.
			tiles.Add(tile);

			tile.origin = loc;

			// A tile might extend across several cells, fill them all
			for (int x = tileBounds.origin.x; x < (tileBounds.origin.x + tileBounds.size.x); x++)
			{
				for (int y = tileBounds.origin.y; y < (tileBounds.origin.y + tileBounds.size.y); y++)
				{
					GetCell(new Vector3Int(x, y, loc.z)).tile = tile;
				}
			}
			layer.EvtCellsBlocked(tileBounds);

			// Don't notify until tile's state is set (or wrong state will be encoded).
			if (!suspendEvents)
			{
				EvtAddTile(tile);
			}

			return true;
		}

		public virtual void RemoveTile(Tile tile)
		{
			if (null == tile) { return; }
			if (tile.board != this) { return; }

			if (!suspendEvents)
			{
				EvtWillRemoveTile(tile);
			}

			Vector3Int loc = tile.origin;
			Rect2Int tileBounds = GetDestTileBounds(tile, loc);

			//selection.Remove(tile);
			tile.board = null;

			// Save value before release tile.
			int depth = tile.origin.z;

			for (int x = tileBounds.origin.x; x < (tileBounds.origin.x + tileBounds.size.x); x++)
			{
				for (int y = tileBounds.origin.y; y < (tileBounds.origin.y + tileBounds.size.y); y++)
				{
					GetCell(new Vector3Int(x, y, depth)).tile = null;
				}
			}
			tiles.Remove(tile);
			layers[depth].EvtCellsUnblocked(tileBounds);

			if (!suspendEvents)
			{
				EvtRemovedTile(tile);
			}
		}

		public virtual void RemoveAllTiles()
		{
			// Avoid mutation error while iterating
			HashSet<Tile> iterTiles = new HashSet<Tile>(tiles);
			foreach (Tile tile in iterTiles)
			{
				RemoveTile(tile);
			}
		}

		#endregion

		#region Utilities

		public Rect2Int GetDestTileBounds(Tile tile, Vector3Int loc)
		{
			Rect2Int result = new Rect2Int(new Vector2Int(loc.x, loc.y), new Vector2Int(tile.size.x, tile.size.y));
			return result;
		}

		public virtual GenericGridCell<Tile> NewCell() { return new GenericGridCell<Tile>(); }

		public GenericGridCell<Tile> GetCell(Vector3Int _loc)
		{
			if (!IsValidLoc(_loc)) { return null; }
			GenericGridCell<Tile> result;

			var loc = new Vector2Int(_loc.x, _loc.y);

			GenericGridLayer<Tile> grid = layers[_loc.z];
			result = grid.GetCell(loc);
			if (null == result)
			{
				result = NewCell();
				result.origin = _loc;
				grid.SetCell(loc, result);
			}

			return result;
		}

		public bool IsCellBlocked(Vector3Int loc)
		{
			if (!IsValidLoc(loc)) { return true; }
			return layers[loc.z].IsCellBlocked(new Vector2Int(loc.x, loc.y));
		}

		public bool IsBlocked(Rect2Int bounds, int depth)
		{
			if (!IsValidLoc(new Vector3Int(0, 0, depth))) { return true; }
			return layers[depth].IsBlocked(bounds);
		}

		public Tile GetTile(Vector3Int loc)
		{
			GenericGridCell<Tile> cell = GetCell(loc);
			if (null != cell)
			{
				return cell.tile;
			}
			return null;
		}

		public void Resize(Vector3Int newSize)
		{
			var newLayerCount = newSize.z;
			var gridSize = new Vector2Int(newSize.x, newSize.y);
			var depth = newSize.z;

			if (depth < 1)
			{
				Debug.LogWarning("Grid depth must be at least 1");
				return;
			}

			// Add new layers.
			while (layers.Count < newLayerCount)
			{
				GenericGridLayer<Tile> layer = new GenericGridLayer<Tile>(gridSize);
				layers.Add(layer);
			}
			if (layers.Count > newLayerCount)
			{
				var diff = layers.Count - newLayerCount;
				layers.RemoveRange(layers.Count - diff, diff);
			}

			foreach (GenericGridLayer<Tile> layer in layers)
			{
				layer.Resize(gridSize);
			}
		}

		public bool IsValidLoc(Vector3Int loc)
		{
			var depth = loc.z;

			if (loc.x < 0 || loc.y < 0 || loc.x >= size.x || loc.y >= size.y)
			{
				return false;
			}
			if (loc.z < 0 || loc.z >= depth)
			{
				return false;
			}
			return true;
		}

		bool IsRowEmpty(Vector3Int row)
		{
			for (int x = 0; x < Width; x++)
			{
				Vector3Int loc = row;
				loc.x = x;
				if (null != GetTile(loc))
				{
					return false;
				}
			}
			return true;
		}

		bool IsColumnEmpty(Vector3Int col)
		{
			for (int y = 0; y < Height; y++)
			{
				Vector3Int loc = col;
				loc.y = y;
				if (null != GetTile(loc))
				{
					return false;
				}
			}
			return true;
		}

		bool IsRowFull(Vector3Int row)
		{
			for (int x = 0; x < Width; x++)
			{
				Vector3Int loc = row;
				loc.x = x;
				if (null == GetTile(loc))
				{
					return false;
				}
			}
			return true;
		}

		bool IsColumnFull(Vector3Int col)
		{
			for (int y = 0; y < Height; y++)
			{
				Vector3Int loc = col;
				loc.y = y;
				if (null == GetTile(loc))
				{
					return false;
				}
			}
			return true;
		}

		int CountTilesInColumn(Vector3Int col)
		{
			int result = 0;
			for (int y = 0; y < Height; y++)
			{
				Tile tile = GetTile(new Vector3Int(col.x, y, col.z));
				if (null == tile)
				{
					continue;
				}
				result++;
				y += tile.size.y - 1;

			}

			return result;
		}

		int CountTilesInRow(Vector3Int col)
		{
			int result = 0;
			for (int x = 0; x < Width; x++)
			{
				Tile tile = GetTile(new Vector3Int(x, col.y, col.z));
				if (null == tile)
				{
					continue;
				}
				result++;
				x += tile.size.x - 1;
			}

			return result;
		}
		#endregion

		#region Events
		public override void EvtUpdate(TimeSlice time)
		{
			base.EvtUpdate(time);

			// Avoid mutation error if tile set changes during update.
			var iterTiles = new HashSet<Tile>(tiles);

			foreach (Tile tile in iterTiles)
			{
				tile.EvtUpdate(time);
			}
		}

		protected virtual void EvtAddTile(Tile tile)
		{
			if (null == broadcaster) { return; }
			broadcaster.Broadcast(new Event(Events.EvtAddTile));
		}

		protected virtual void EvtWillRemoveTile(Tile tile)
		{
			if (null == broadcaster) { return; }
			broadcaster.Broadcast(new Event(Events.EvtWillRemoveTile));
		}

		protected virtual void EvtRemovedTile(Tile tile)
		{
			if (null == broadcaster) { return; }
			broadcaster.Broadcast(new Event(Events.EvtRemovedTile));
		}

		protected virtual void EvtTileModified(Tile tile)
		{

		}
		#endregion

		#region Axial
		/*
		  AXIAL:
		  Axial directions can be used to navigate a grid by moving in a specific
		  direction (North, Northeast, etc.)
		 */

		// 	ORDER: top-left, running clockwise.
		protected static readonly Vector2Int[] GridNeighborAxialLocs = {
			new Vector2Int(-1, -1),
			new Vector2Int(0, -1),
			new Vector2Int(1, -1),
			new Vector2Int(1, 0),
			new Vector2Int(1, 1),
			new Vector2Int(0, 1),
			new Vector2Int(-1, 1),
			new Vector2Int(-1, 0)
		};

		/// <summary>
		/// Returns the number of axial directions from one cell to another (North, Northeast, etc.)
		/// </summary>
		public virtual int NumAxialDirections() { return 8; }

		bool DoesAxialIndexMatchType(int index, AxialType type)
		{
			bool result = true;

			switch (type)
			{
				case AxialType.AxialEdge:
					result = (index % 2 != 0);
					break;
				default:
					break;
			}

			return result;
		}

		public Vector2Int GetAxial(int index) {
			Vector2Int result = new Vector2Int(0, 0);
			return index < 0 || index > (GridNeighborAxialLocs.Length - 1) ? result : GridNeighborAxialLocs[index];
		}

		public int GetAxialIndex(Vector2Int axial) {
			for (int i = 0; i < NumAxialDirections(); i++) {
				Vector2Int axialOffset = GridNeighborAxialLocs[i];
				if (axialOffset == axial) {
					return i;
				}
			}
	
			return -1;	// Invalid.
		}

		public int GetNextAxialIndex(int axialIndex, AxialDirection dir) {
	
			switch (dir) {
				case AxialDirection.Right:
					axialIndex++;
					break;
				default:
					axialIndex--;
					if (axialIndex < 0) {
						axialIndex = NumAxialDirections() - 1;
					}
					break;
			}
	
			axialIndex %= NumAxialDirections();
	
			return axialIndex;
		}

		Vector2Int GetNextAxial(int axialIndex, AxialDirection dir) {
			int nextIndex = GetNextAxialIndex(axialIndex, dir);
			return GridNeighborAxialLocs[nextIndex];
		}

		Vector3Int OffsetLoc(Vector3Int loc, Vector2Int offset)
		{
			Vector3Int result = loc;
			result.x += offset.x;
			result.y += offset.y;
			return result;
		}

		public HashSet<Tile> CollectNeighbors(Vector3Int loc)
		{
			var neighbors = new HashSet<Tile>();

			for (int i = 0; i < NumAxialDirections(); i++)
			{
				Vector2Int axialOffset = GridNeighborAxialLocs[i];
				Vector3Int neighborLoc = OffsetLoc(loc, axialOffset);
				Tile neighbor = GetTile(neighborLoc);
				if (null != neighbor)
				{
					neighbors.Add(neighbor);
				}
			}

			return neighbors;
		}

		/// <summary>
		/// Returns true if the two tiles touch (depending on AxialType)
		/// 
		/// NOTE: this currently only works with 1x1 tiles
		/// </summary>
		/// <param name="tile1"></param>
		/// <param name="tile2"></param>
		/// <param name="axialType"></param>
		/// <returns></returns>
		public bool DoTilesTouch(Tile tile1, Tile tile2, AxialType axialType)
		{
			if (null == tile1 || null == tile2)
			{
				return false;
			}
			if (tile1 == tile2)
			{
				Debug.LogWarning("WARNING. DoTilesTouch can't compare the same tile.");
				return false;
			}

			bool result = false;

			for (int i = 0; i < NumAxialDirections(); i++)
			{
				Vector2Int axialOffset = GetAxial(i);
				Vector3Int neighborLoc = OffsetLoc(tile1.origin, axialOffset);
				if (!DoesAxialIndexMatchType(i, axialType))
				{
					continue;
				}

				if (neighborLoc == tile2.origin)
				{
					return true;
				}
			}

			return result;

		}
		#endregion
	}
}

