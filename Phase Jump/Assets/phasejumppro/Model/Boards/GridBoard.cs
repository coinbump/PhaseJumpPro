using UnityEngine;
using System.Collections;

namespace PJ
{
	using Tile = PJ.AbstractGridTile;
	
	class GridBoard : Core
	{
		public int depth = 1;
		public Vector2Int size = new Vector2Int(0, 0);

		struct Events
		{
			public const string EvtAddTile = "tile_add";
			public const string EvtWillRemoveTile = "tile_will_remove";
		}

		// TODO: Finish this
		public int Width() { return 0; }
		public int Height() { return 0; }
		public Tile GetTile(Vector3Int loc) { return null; }
		public GridCell GetCell(Vector3Int loc) { return null; }

		#region Utilities
		public bool IsValidLoc(Vector3Int loc)
		{
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

		public bool IsCellBlocked(Vector3Int loc) {
			if (!IsValidLoc(loc)) { return true; }

			GridCell cell = GetCell(loc);
			if (null == cell) {
				return false;	// Nothing there.
			}
			if (null != cell.tile && cell.tile.isGhost) {
				return false;	// Ghost tile.
			}
			return null != cell.tile;
		}

		bool IsRowEmpty(Vector3Int row)
		{
			for (int x = 0; x < Width(); x++)
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
			for (int y = 0; y < Height(); y++)
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
			for (int x = 0; x < Width(); x++)
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
			for (int y = 0; y < Height(); y++)
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
			for (int y = 0; y < Height(); y++)
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
			for (int x = 0; x < Width(); x++)
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

//virtual PJ_BoardGrid newBoardGrid(int width, int height, BoardDistro distro)
//{
//	return new PJ_BoardGrid(this, width, height, distro);
//}


//public:
//	bool mSuspendNotify;    // TRUE: suspend notification for add/remove events.
//Vector2Int size;
//GridVector mGrids;
//TileSet tiles;
//PJ_TSelection<PJ_GridTile> mSelection;
//BoardDistro mDistro;    // OPTIMIZE: turn off distro tracking if you need more speed.

//void Build(int width, int height, int depth)
//{
//	if (depth < 1)
//	{
//		PJLog("ERROR. Grid depth must be >= 1.");
//		depth = 1;
//	}

//	mSelection.Clear();
//	RemoveAllTiles();
//	mGrids.RemoveAll();

//	size = Vector2Int(width, height);
//	depth = depth;

//	for (int i = 0; i < depth; i++)
//	{
//		PJ_BoardGridPtr grid(newBoardGrid(width, height, mDistro));
//		mGrids.Add(grid);
//	}
//}

///*
//	IsValidLoc

//	OPTIMIZE: avoid calling methods, this function is called very often.

// */
	
//	virtual GridCell NewCell() { return new GridCell; }
	
//	GridCell GetCell(Vector3Int loc) {
//		if (!IsValidLoc(loc)) { return null; }
//		GridCell result = null;

//PJ_BoardGrid grid = mGrids[loc.z].get();
//result = grid.GetCell(loc);
//		if (null == result) {
//			result = NewCell();
//result.mLoc = loc;
//			grid.SetCell(loc, result);
//		}
		
//		return result;
		
//	}
	
//	bool IsCellBlocked(Vector3Int loc) {
//		if (!IsValidLoc(loc)) { return true; }
//		return mGrids[loc.z].IsCellBlocked(loc);
//	}
	
//	bool IsBlocked(PJ_VecRect2Int bounds, int depth) {
//		for (int x = bounds.left(); x <= bounds.right(); x++) {
//			for (int y = bounds.top(); y <= bounds.bottom(); y++) {
//				if (IsCellBlocked(Vector3Int(x, y, depth))) {
//					return true;
//				}
//			}
//		}
		
//		return false;
//	}
	
//	PJ_GridTile GetTile(Vector3Int loc) {
//		GridCell cell = GetCell(loc);
//		if (null != cell) {
//			return static_cast<PJ_GridTile*>(cell.tile);
//		}
//		return null;
		
//	}
	
//	PJ_VecRect2Int GetDestTileBounds(PJ_GridTile tile, Vector3Int loc)
//{

//	PJ_VecRect2Int result(loc.x, loc.y);
//	result.SetSize(tile.size.x, tile.size.y);
//	return result;
//}

//bool PutTile(PJ_GridTile tile, Vector3Int loc)
//{
//	if (loc.z < 0 || loc.z >= static_cast<int>(mGrids.size())) { return false; }
//	if (null == tile) { return false; }
//	if (tile.size.x < 1 || tile.size.y < 1)
//	{
//		PJLog("ERROR. Invalid tile size %d, %d", tile.size.x, tile.size.y);
//		return false;
//	}

//	PJ_VecRect2Int tileBounds = GetDestTileBounds(tile, loc);
//	if (IsBlocked(tileBounds, loc.z))
//	{
//		return false;
//	}

//	tiles.Put(tile);

//	tile.mBoard = this;
//	tile.mGrid = mGrids[loc.z].get();
//	tile.mOrigin = loc;

//	for (int x = tileBounds.left(); x <= tileBounds.right(); x++)
//	{
//		for (int y = tileBounds.top(); y <= tileBounds.bottom(); y++)
//		{
//			GetCell(Vector3Int(x, y, loc.z)).tile = tile;
//		}
//	}
//	tile.mGrid.evtCellsBlocked(tileBounds);

//	// Don't notify until tile's state is set (or wrong state will be encoded).
//	if (!mSuspendNotify)
//	{
//		evtAddTile(tile);
//	}

//	return true;
//}

//void RemoveTile(PJ_GridTile tile)
//{
//	if (null == tile) { return; }
//	if (tile.mBoard != this) { return; }

//	if (!mSuspendNotify)
//	{
//		evtRemoveTile(tile);    // Before release
//	}

//	Vector3Int loc = tile.mOrigin;
//	PJ_VecRect2Int tileBounds = GetDestTileBounds(tile, loc);

//	mSelection.Remove(tile);
//	tile.mBoard = null;

//	// Save value before release tile.
//	int depth = tile.mOrigin.z;

//	for (int x = tileBounds.left(); x <= tileBounds.right(); x++)
//	{
//		for (int y = tileBounds.top(); y <= tileBounds.bottom(); y++)
//		{
//			GetCell(Vector3Int(x, y, depth)).tile = null;
//		}
//	}
//	tiles.Remove(tile);
//	mGrids[depth].evtCellsUnblocked(tileBounds);

//	if (!mSuspendNotify)
//	{
//		mBroadcaster.Broadcast(kEvtTileRemoved);
//	}
//}

//virtual void RemoveAllTiles()
//{
//	set<PJ_GridTile*> iterTiles = tiles;
//	FOR_I(set<PJ_GridTile*>, iterTiles) {
//		RemoveTile(*i);
//	}
//}


//virtual void evtUpdate(PJ_TimeSlice const& task);

//// GO:
//int Width() { return size.x; }
//	int Height() { return size.y; }
//	int Depth() { return depth; }
	
//};


///*
//	mapDistroLocSize
 
//	Maps which cells are an open slot for a tile of the specified size.
	
// */
//void mapDistroLocSize(Vector3Int loc, Vector2Int size, bool testBlocked)
//{
//	if ((loc.x + size.x - 1) >= Width() ||
//		(loc.y + size.y - 1) >= Height())
//	{

//		return; // Invalid location.
//	}

//	PJ_VecRect2Int bounds(loc.x, loc.y);
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
//void evtCellsUnblocked(PJ_VecRect2Int const& blocked)
//{
//	switch (mDistro)
//	{
//		case BoardDistro::Track:
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
//				//				PJ_VecRect2Int	thisBounds(x,y);
//				//				thisBounds.SetSize(size);
//				//				assert(thisBounds.TestIntersect(blocked));
//#endif

//				mapDistroLocSize(Vector3Int(x, y), size, true);
//			}
//		}
//	}

//}

///*
//	FindRandomLocForTile
 
//	If tracking.
 
// */
//Vector3Int FindRandomLocForTile(Vector2Int tileSize)
//{
//	Vector3Int result(-1, -1);  // Invalid.
//	switch (mDistro)
//	{
//		case BoardDistro::Track:
//			break;
//		default:
//			PJLog("ERROR. Can't use FindRandomLocForTile. Not tracking distro slots");
//			return result;
//			break;
//	}

//	buildMapsForSize(tileSize); // Update distro maps (if needed).
//	DistroCellMap::iterator cellI = getDistroCellIterator(tileSize);  // Always returns an iterator
//	size_t numCells = cellI.second.size();

//	if (numCells > 0)
//	{
//		size_t choice = PJ_Random::Choice(numCells);

//		// NOTE: possibly optimize this in the future. We need set for fast remove, but random
//		// choice isn't as efficient. (optimizing remove is more important).
//		CellSet::iterator chooseI = cellI.second.begin();
//		for (int choose = 0; choose < choice; choose++)
//		{
//			chooseI++;
//		}
//		result = (*chooseI).mLoc;
//	}

//	return result;

//}


//void MoveTile(Tile tile, Vector3Int newLoc)
//{
//	if (newLoc.z != tile.mOrigin.z)
//	{
//		PJLog("ERROR. MoveTile only moves within the same z grid.");
//		return;
//	}

//	// Don't notify, we're just moving the tile, not removing it.
//	PJ_TChangeAndRestore<bool> altSuspendNotify(mSuspendNotify, true);

//	Vector3Int firstLoc = tile.mOrigin;

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
//	PJ_TChangeAndRestore<bool> altSuspendNotify(mSuspendNotify, true);

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

//	for (int y = 0; y < Height(); y++)
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
//			Vector3Int oldLocB = tileB.mOrigin;
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
//	PJ_TChangeAndRestore<bool> altSuspendNotify(mSuspendNotify, true);

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

//	for (int x = 0; x < Width(); x++)
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
//			Vector3Int oldLocB = tileB.mOrigin;
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
//	PJ_TChangeAndRestore<bool> altSuspendNotify(mSuspendNotify, true);

//	vector<PJ_GridTile*> tiles;
//	for (int y = 0; y < Height(); y++)
//	{
//		PJ_GridTile tile = GetTile(Vector3Int(a.x, y, a.z));
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
//		offset = Height() + (offset % Height());
//	}

//	FOR_CONST_I(vector<PJ_GridTile*>, tiles) {
//		PJ_GridTile tile = *i;
//		Vector3Int newLoc = tile.mOrigin;
//		newLoc.y += offset;

//		if (wrap)
//		{
//			newLoc.y %= Height();
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
//	PJ_TChangeAndRestore<bool> altSuspendNotify(mSuspendNotify, true);

//	vector<PJ_GridTile*> tiles;
//	for (int x = 0; x < Width(); x++)
//	{
//		PJ_GridTile tile = GetTile(Vector3Int(x, a.y, a.z));
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
//		offset = Width() + (offset % Width());
//	}

//	FOR_CONST_I(vector<PJ_GridTile*>, tiles) {
//		PJ_GridTile tile = *i;
//		Vector3Int newLoc = tile.mOrigin;
//		newLoc.x += offset;

//		if (wrap)
//		{
//			newLoc.x %= Width();
//		}

//		if (!PutTile(tile, newLoc))
//		{
//			//			PJLog("ERROR. SlideRow failed at %d, %d, %d", newLoc.x, newLoc.y, newLoc.z);
//			delete tile;
//		}
//	}

//}

//void evtCellsBlocked(PJ_VecRect2Int const& blocked)
//{
//	switch (mDistro)
//	{
//		case BoardDistro::Track:
//			break;
//		default:
//			return;
//			break;
//	}

//	// Remove all slots that have now been blocked.
//	FOR_I(set<Vector2Int>, mDistroSizes) {
//		Vector2Int size = *i;
//		DistroCellMap::iterator cellIter = getDistroCellIterator(size);

//		for (int x = blocked.left() - (size.x - 1); x <= blocked.right(); x++)
//		{
//			for (int y = blocked.top() - (size.y - 1); y <= blocked.bottom(); y++)
//			{
//				if (!IsValidLoc(Vector3Int(x, y))) { continue; }

//# ifdef __DEBUG__
//				//				PJ_VecRect2Int thisBounds(x,y);
//				//				thisBounds.SetSize(size);
//				//				assert(thisBounds.TestIntersect(blocked));
//#endif

//				GridCell cell = GetCell(Vector3Int(x, y));
//				cellIter.second.erase(cell);
//			}
//		}
//	}

//}


//bool IsBlocked(PJ_VecRect2Int bounds) {
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

//	int width = Width();
//	int height = Height();
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
//DistroCellMap::iterator getDistroCellIterator(Vector2Int size)
//{
//	DistroCellMap::iterator i = mDistroCellMap.find(size);
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
//		Vector3Int neighborLoc = OffsetLoc(tile.mOrigin, axialOffset);
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
//		Vector3Int neighborLoc = OffsetLoc(tile1.mOrigin, axialOffset);
//		if (!DoesAxialIndexMatchType(i, axialType))
//		{
//			continue;
//		}

//		if (neighborLoc == tile2.mOrigin)
//		{
//			return true;
//		}
//	}

//	return result;

//}

//bool DoesAxialIndexMatchType(int index, AxialType type) {
//	bool result = true;
	
//	switch (type) {
//		case AxialType::AxialEdge:
//			result = (index % 2 != 0);
//			break;
//		default:
//			break;
//	}
	
//	return result;
//}



//void evtUpdate(PJ_TimeSlice const& task)
//{

//	// Avoid mutation error if tile set changes during update.
//	set<PJ_GridTile*> iterTiles = tiles;
//	FOR_CONST_I(TileSet, iterTiles) {
//		(*i).evtUpdate(task);
//	}

//}

//#pragma mark - PJ_GridTile

//void PJ_GridTile::evtModified()
//{
//	if (null != mBoard) mBoard.evtTileModified(this);
//}
