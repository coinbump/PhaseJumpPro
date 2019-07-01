using UnityEngine;
using System.Collections;

namespace PJ
{
	using Tile = AbstractGridTile;

	/// <summary>
	/// Stores model data for each layer of the grid board
	/// </summary>
	class GridLayer : GenericGrid<GridCell>
	{
		// FUTURE: add support for cell sets, fast distribution (for populating grid worlds).

		public bool IsCellBlocked(Vector2Int loc) {
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

		public bool IsBlocked(Rect2Int bounds) {
			for (int x = bounds.origin.x; x < (bounds.origin.x + bounds.size.x); x++) {
				for (int y = bounds.origin.y; y < (bounds.origin.y + bounds.size.y); y++) {
					if (IsCellBlocked(new Vector2Int(x, y))) {
						return true;
					}
				}
			}
	
			return false;
		}

		public GridLayer(Vector2Int size)
			: base(size)
		{
		}

		#region Events
		public virtual void EvtCellsBlocked(Rect2Int blocked) { }
		public virtual void EvtCellsUnblocked(Rect2Int blocked) { }
		#endregion
	}
}


// TODO: port this C++ code to C#
//virtual GridLayer newBoardGrid(int width, int height, BoardDistro distro)
//{
//	return new GridLayer(this, width, height, distro);
//}

//public:
//TileSet tiles;
//PJ_TSelection<Tile> mSelection;
//BoardDistro mDistro;    // OPTIMIZE: turn off distro tracking if you need more speed.


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