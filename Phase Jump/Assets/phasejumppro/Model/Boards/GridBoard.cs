using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace PJ
{
	using Tile = PJ.AbstractGridTile;

	class GridBoard : Core
	{
		#region Constants
		struct Events
		{
			public const string EvtAddTile = "tile_add";
			public const string EvtWillRemoveTile = "tile_will_remove";
		}
		#endregion

		#region Fields
		protected Vector3Int size = new Vector3Int(0, 0, 0);
		protected List<GridLayer> layers = new List<GridLayer>();
		protected bool suspendEvents;	// If true, don't broadcast events
		#endregion

		#region Properties
		public int Width
		{
			get
			{
				return size.x;
			}
		}
		public int Height
		{
			get
			{
				return size.y;
			}
		}
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

			// TODO: Finish porting from C++
			//tiles.Put(tile);

			//tile.mBoard = this;
			//tile.mGrid = mGrids[loc.z].get();
			//tile.origin = loc;

			//for (int x = tileBounds.origin.x; x < (tileBounds.origin.x + tileBounds.size.x); x++)
			//{
			//	for (int y = tileBounds.origin.y; y < (tileBounds.origin.y + tileBounds.size.y); y++)
			//	{
			//		GetCell(new Vector3Int(x, y, loc.z)).tile = tile;
			//	}
			//}
			//tile.mGrid.evtCellsBlocked(tileBounds);

			// Don't notify until tile's state is set (or wrong state will be encoded).
			if (!suspendEvents)
			{
				EvtAddTile(tile);
			}

			return true;
		}
		#endregion

		#region Utilities

		public Rect2Int GetDestTileBounds(Tile tile, Vector3Int loc)
		{
			Rect2Int result = new Rect2Int(new Vector2Int(loc.x, loc.y), new Vector2Int(tile.size.x, tile.size.y));
			return result;
		}

		public virtual GridCell NewCell() { return new GridCell(); }

		public GridCell GetCell(Vector3Int _loc)
		{
			if (!IsValidLoc(_loc)) { return null; }
			GridCell result = null;

			var loc = new Vector2Int(_loc.x, _loc.y);

			GridLayer grid = layers[_loc.z];
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
			GridCell cell = GetCell(loc);
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
				GridLayer layer = new GridLayer(gridSize);
				layers.Add(layer);
			}
			if (layers.Count > newLayerCount)
			{
				var diff = layers.Count - newLayerCount;
				layers.RemoveRange(layers.Count - diff, diff);
			}

			foreach (GridLayer layer in layers)
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
		#endregion
	}
}


// TODO: port this C++ code to C#
//virtual GridLayer newBoardGrid(int width, int height, BoardDistro distro)
//{
//	return new GridLayer(this, width, height, distro);
//}

//public:
//	bool suspendEvents;    // TRUE: suspend notification for add/remove events.
//TileSet tiles;
//PJ_TSelection<Tile> mSelection;
//BoardDistro mDistro;    // OPTIMIZE: turn off distro tracking if you need more speed.

///*
//	IsValidLoc

//	OPTIMIZE: avoid calling methods, this function is called very often.

// */



//void RemoveTile(Tile tile)
//{
//	if (null == tile) { return; }
//	if (tile.mBoard != this) { return; }

//	if (!suspendEvents)
//	{
//		evtRemoveTile(tile);    // Before release
//	}

//	Vector3Int loc = tile.origin;
//	Rect2Int tileBounds = GetDestTileBounds(tile, loc);

//	mSelection.Remove(tile);
//	tile.mBoard = null;

//	// Save value before release tile.
//	int depth = tile.origin.z;

//	for (int x = tileBounds.left(); x <= tileBounds.right(); x++)
//	{
//		for (int y = tileBounds.top(); y <= tileBounds.bottom(); y++)
//		{
//			GetCell(Vector3Int(x, y, depth)).tile = null;
//		}
//	}
//	tiles.Remove(tile);
//	layers[depth].evtCellsUnblocked(tileBounds);

//	if (!suspendEvents)
//	{
//		mBroadcaster.Broadcast(kEvtTileRemoved);
//	}
//}

//virtual void RemoveAllTiles()
//{
//	set<Tile*> iterTiles = tiles;
//	FOR_I(set<Tile*>, iterTiles) {
//		RemoveTile(*i);
//	}
//}

//// GO:
//int Width { return size.x; }
//	int Height { return size.y; }
//	int Depth() { return depth; }

//};


///*
//	mapDistroLocSize

//	Maps which cells are an open slot for a tile of the specified size.

// */
//void mapDistroLocSize(Vector3Int loc, Vector2Int size, bool testBlocked)
//{
//	if ((loc.x + size.x - 1) >= Width ||
//		(loc.y + size.y - 1) >= Height)
//	{

//		return; // Invalid location.
//	}

//	Rect2Int bounds(loc.x, loc.y);
//	bounds.SetSize(size[0], size[1]);

//	bool isBlocked = false;
//	if (testBlocked)
//	{
//		isBlocked = IsBlocked(bounds);
//	}

//	GridCell cell = GetCell(loc);
//	if (null == cell)
//	{
//		cell = new GridCell(loc);
//		SetCell(loc, cell);
//	}
//	if (!isBlocked)
//	{
//		getDistroCellIterator(size).second.insert(cell);
//	}
//	else
//	{
//		getDistroCellIterator(size).second.erase(cell);
//	}

//}

///*
//	evtCellsUnblocked

//	Called when a tile is removed, updates the open slots map.

// */
//void evtCellsUnblocked(Rect2Int const& blocked)
//{
//	switch (mDistro)
//	{
//		case BoardDistro.Track:
//			break;
//		default:
//			return;
//			break;
//	}

//	// Add back slots that are now unblocked.
//	FOR_I(set<Vector2Int>, mDistroSizes) {
//		Vector2Int size = *i;

//		// FUTURE: this could be further optimized since we know that if size is smaller than the
//		// unblocked bounds, as long as size fits we don't have to test IsBlocked (good enough for now).
//		for (int x = blocked.left() - (size.x - 1); x <= blocked.right(); x++)
//		{
//			for (int y = blocked.top() - (size.y - 1); y <= blocked.bottom(); y++)
//			{
//				if (!IsValidLoc(Vector3Int(x, y))) { continue; }

//# ifdef __DEBUG__
//				//				Rect2Int	thisBounds(x,y);
//				//				thisBounds.SetSize(size);
//				//				assert(thisBounds.TestIntersect(blocked));
//#endif

//				mapDistroLocSize(Vector3Int(x, y), size, true);
//			}
//		}
//	}

//}

///*
//	FindRandooriginForTile

//	If tracking.

// */
//Vector3Int FindRandooriginForTile(Vector2Int tileSize)
//{
//	Vector3Int result(-1, -1);  // Invalid.
//	switch (mDistro)
//	{
//		case BoardDistro.Track:
//			break;
//		default:
//			PJLog("ERROR. Can't use FindRandooriginForTile. Not tracking distro slots");
//			return result;
//			break;
//	}

//	buildMapsForSize(tileSize); // Update distro maps (if needed).
//	DistroCellMap.iterator cellI = getDistroCellIterator(tileSize);  // Always returns an iterator
//	size_t numCells = cellI.second.size();

//	if (numCells > 0)
//	{
//		size_t choice = PJ_Random.Choice(numCells);

//		// NOTE: possibly optimize this in the future. We need set for fast remove, but random
//		// choice isn't as efficient. (optimizing remove is more important).
//		CellSet.iterator chooseI = cellI.second.begin();
//		for (int choose = 0; choose < choice; choose++)
//		{
//			chooseI++;
//		}
//		result = (*chooseI).origin;
//	}

//	return result;

//}


//void MoveTile(Tile tile, Vector3Int newLoc)
//{
//	if (newLoc.z != tile.origin.z)
//	{
//		PJLog("ERROR. MoveTile only moves within the same z grid.");
//		return;
//	}

//	// Don't notify, we're just moving the tile, not removing it.
//	PJ_TChangeAndRestore<bool> altSuspendNotify(suspendEvents, true);

//	Vector3Int firstLoc = tile.origin;

//	pjRetain(tile);
//	RemoveTile(tile);
//	if (!PutTile(tile, newLoc))
//	{
//		PJLog("ERROR. MoveTile can't move to loc %d, %d, %d", newLoc.x, newLoc.y, newLoc.z);
//		PutTile(tile, firstLoc);
//	}

//}

///*
// 	SwapColumn

// 	USAGE: assumes that all tiles are uniform in size. If the grid has irregular tile sizes, this
// 	can fail and leak memory.

// */
//bool SwapColumn(Vector3Int a, Vector3Int b)
//{
//	// Don't notify, we're just moving the tiles.
//	PJ_TChangeAndRestore<bool> altSuspendNotify(suspendEvents, true);

//	if (a.z != b.z)
//	{
//		PJLog("ERROR. SwapColumn only swaps within the same Z grid.");
//		return false;
//	}
//	if (a.x == b.x)
//	{
//		PJLog("ERROR. SwapColumn can't swap column %d with itself.", a.x);
//		return false;
//	}

//	for (int y = 0; y < Height; y++)
//	{
//		Tile tileA = GetTile(Vector3Int(a.x, y, a.z));
//		if (null != tileA)
//		{
//			pjRetain(tileA);
//			RemoveTile(tileA);
//		}

//		Tile tileB = GetTile(Vector3Int(b.x, y, b.z));
//		if (null != tileB)
//		{
//			Vector3Int oldLocB = tileB.origin;
//			pjRetain(tileB);
//			RemoveTile(tileB);
//			Vector3Int newLoc(a.x, y, a.z);
//			if (!PutTile(tileB, newLoc))
//			{
//				PJLog("ERROR. SwapColumn didn't fit at %d, %d, %d.", newLoc.x, newLoc.y, newLoc.z);
//				PutTile(tileB, oldLocB);
//				return false;
//			}
//		}

//		if (null != tileA)
//		{
//			Vector3Int newLoc(b.x, y, b.z);
//			if (!PutTile(tileA, newLoc))
//			{
//				PJLog("ERROR. SwapColumn didn't fit at %d, %d, %d.", newLoc.x, newLoc.y, newLoc.z);
//				// MEMORY LEAK:
//				return false;
//			}
//		}
//	}

//	return true;

//}

///*
//	SwapRow

//	USAGE: assumes that all tiles are uniform in size. If the grid has irregular tile sizes, this
//	can fail and leak memory.

// */
//bool SwapRow(Vector3Int a, Vector3Int b)
//{
//	// Don't notify, we're just moving the tiles.
//	PJ_TChangeAndRestore<bool> altSuspendNotify(suspendEvents, true);

//	if (a.z != b.z)
//	{
//		PJLog("ERROR. SwapRow only swaps within the same Z grid.");
//		return false;
//	}
//	if (a.y == b.y)
//	{
//		PJLog("ERROR. SwapRow can't swap row %d with itself.", a.y);
//		return false;
//	}

//	for (int x = 0; x < Width; x++)
//	{
//		Tile tileA = GetTile(Vector3Int(x, a.y, a.z));
//		if (null != tileA)
//		{
//			pjRetain(tileA);
//			RemoveTile(tileA);
//		}

//		Tile tileB = GetTile(Vector3Int(x, b.y, b.z));
//		if (null != tileB)
//		{
//			Vector3Int oldLocB = tileB.origin;
//			pjRetain(tileB);
//			RemoveTile(tileB);
//			Vector3Int newLoc(x, a.y, a.z);
//			if (!PutTile(tileB, newLoc))
//			{
//				PJLog("ERROR. SwapRow didn't fit at %d, %d, %d.", newLoc.x, newLoc.y, newLoc.z);
//				PutTile(tileB, oldLocB);
//				return false;
//			}
//		}

//		if (null != tileA)
//		{
//			Vector3Int newLoc(x, b.y, b.z);
//			if (!PutTile(tileA, newLoc))
//			{
//				PJLog("ERROR. SwapRow didn't fit at %d, %d, %d.", newLoc.x, newLoc.y, newLoc.z);
//				// MEMORY LEAK:
//				return false;
//			}
//		}
//	}

//	return true;

//}

///*
//	SlideColumn

// 	Slides the column and wraps the tiles.

//	USAGE: assumes that all tiles are uniform in size. If the grid has irregular tile sizes, this
//	can fail and leak memory.

// */
//void SlideColumn(Vector3Int a, int offset, bool wrap)
//{
//	// Don't notify, we're just moving the tiles.
//	PJ_TChangeAndRestore<bool> altSuspendNotify(suspendEvents, true);

//	vector<Tile*> tiles;
//	for (int y = 0; y < Height; y++)
//	{
//		Tile tile = GetTile(Vector3Int(a.x, y, a.z));
//		if (null == tile)
//		{
//			continue;
//		}

//		tiles.push_back(tile);
//		pjRetain(tile);
//		RemoveTile(tile);
//	}

//	if (wrap && offset < 0)
//	{
//		offset = Height + (offset % Height);
//	}

//	FOR_CONST_I(vector<Tile*>, tiles) {
//		Tile tile = *i;
//		Vector3Int newLoc = tile.origin;
//		newLoc.y += offset;

//		if (wrap)
//		{
//			newLoc.y %= Height;
//		}

//		if (!PutTile(tile, newLoc))
//		{
//			//			PJLog("ERROR. SlideColumn failed at %d, %d, %d", newLoc.x, newLoc.y, newLoc.z);
//			delete tile;
//		}
//	}

//}

///*
//	SlideColumn

//	Slides the column and wraps the tiles.

//	USAGE: assumes that all tiles are uniform in size. If the grid has irregular tile sizes, this
//	can fail and leak memory.

// */
//void SlideRow(Vector3Int a, int offset, bool wrap)
//{
//	// Don't notify, we're just moving the tiles.
//	PJ_TChangeAndRestore<bool> altSuspendNotify(suspendEvents, true);

//	vector<Tile*> tiles;
//	for (int x = 0; x < Width; x++)
//	{
//		Tile tile = GetTile(Vector3Int(x, a.y, a.z));
//		if (null == tile)
//		{
//			continue;
//		}

//		tiles.push_back(tile);
//		pjRetain(tile);
//		RemoveTile(tile);
//	}

//	if (wrap && offset < 0)
//	{
//		offset = Width + (offset % Width);
//	}

//	FOR_CONST_I(vector<Tile*>, tiles) {
//		Tile tile = *i;
//		Vector3Int newLoc = tile.origin;
//		newLoc.x += offset;

//		if (wrap)
//		{
//			newLoc.x %= Width;
//		}

//		if (!PutTile(tile, newLoc))
//		{
//			//			PJLog("ERROR. SlideRow failed at %d, %d, %d", newLoc.x, newLoc.y, newLoc.z);
//			delete tile;
//		}
//	}

//}

//void evtCellsBlocked(Rect2Int const& blocked)
//{
//	switch (mDistro)
//	{
//		case BoardDistro.Track:
//			break;
//		default:
//			return;
//			break;
//	}

//	// Remove all slots that have now been blocked.
//	FOR_I(set<Vector2Int>, mDistroSizes) {
//		Vector2Int size = *i;
//		DistroCellMap.iterator cellIter = getDistroCellIterator(size);

//		for (int x = blocked.left() - (size.x - 1); x <= blocked.right(); x++)
//		{
//			for (int y = blocked.top() - (size.y - 1); y <= blocked.bottom(); y++)
//			{
//				if (!IsValidLoc(Vector3Int(x, y))) { continue; }

//# ifdef __DEBUG__
//				//				Rect2Int thisBounds(x,y);
//				//				thisBounds.SetSize(size);
//				//				assert(thisBounds.TestIntersect(blocked));
//#endif

//				GridCell cell = GetCell(Vector3Int(x, y));
//				cellIter.second.erase(cell);
//			}
//		}
//	}

//}

//bool IsBlocked(Rect2Int bounds) {
//	for (int x = bounds.left(); x <= bounds.right(); x++) {
//		for (int y = bounds.top(); y <= bounds.bottom(); y++) {
//			if (IsCellBlocked(Vector3Int(x, y))) {
//				return true;
//			}
//		}
//	}

//	return false;
//}

//void buildMapsForSize(Vector2Int size)
//{
//	// FUTURE: support resize of the board.
//	if (mDistroSizes.find(size) != mDistroSizes.end())
//	{
//		return;
//	}
//	mDistroSizes.insert(size);  // We are now tracking slots for this tile size.
//	PJLog("Built map for size %d, %d", size.x, size.y);

//	int width = Width;
//	int height = Height;
//	for (int x = 0; x <= (width - size.x); x++)
//	{
//		for (int y = 0; y <= (height - size.y); y++)
//		{
//			mapDistroLocSize(Vector3Int(x, y), size, true);
//		}
//	}

//}

///*
//	getDistroCellIterator

//	RETURNS: a set of cells that are open slots available for the specified tile size.

// */
//DistroCellMap.iterator getDistroCellIterator(Vector2Int size)
//{
//	DistroCellMap.iterator i = mDistroCellMap.find(size);
//	if (mDistroCellMap.end() == i)
//	{
//		CellSet newSet;
//		i = mDistroCellMap.insert(make_pair(size, newSet)).first;
//	}

//	return i;

//}

//void CollectNeighbors(Tile tile, vector<Tile*>& neighbors)
//{

//	neighbors.clear();

//	for (int i = 0; i < NumAxialDirections(); i++)
//	{
//		Vector2Int axialOffset = GridNeighborAxialLocs[i];
//		Vector3Int neighborLoc = OffsetLoc(tile.origin, axialOffset);
//		Tile neighbor = static_cast<Tile*>(GetTile(neighborLoc));
//		if (null != neighbor)
//		{
//			neighbors.push_back(neighbor);
//		}
//	}

//}

//bool DoTilesTouch(Tile tile1, Tile tile2, AxialType axialType)
//{
//	if (null == tile1 || null == tile2)
//	{
//		return false;
//	}
//	if (tile1 == tile2)
//	{
//		PJLog("ERROR. DoTilesTouch can't compare the same tile.");
//		return false;
//	}

//	bool result = false;

//	for (int i = 0; i < NumAxialDirections(); i++)
//	{
//		Vector2Int axialOffset = GetAxial(i);
//		Vector3Int neighborLoc = OffsetLoc(tile1.origin, axialOffset);
//		if (!DoesAxialIndexMatchType(i, axialType))
//		{
//			continue;
//		}

//		if (neighborLoc == tile2.origin)
//		{
//			return true;
//		}
//	}

//	return result;

//}

//void evtUpdate(PJ_TimeSlice const& task)
//{

//	// Avoid mutation error if tile set changes during update.
//	set<Tile*> iterTiles = tiles;
//	FOR_CONST_I(TileSet, iterTiles) {
//		(*i).evtUpdate(task);
//	}

//}

//#pragma mark - Tile

//void Tile.evtModified()
//{
//	if (null != mBoard) mBoard.evtTileModified(this);
//}
