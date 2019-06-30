using UnityEngine;
using System.Collections;

namespace PJ
{
	using Tile = PJ.AbstractGridTile;

	class GridBoard : Core
	{
		struct Events
		{
			public const string EvtAddTile = "tile_add";
			public const string EvtRemoveTile = "tile_remove";
		}

		virtual protected void EvtAddTile(Tile tile)
		{
			if (null == broadcaster) { return; }
			broadcaster.Broadcast(new Event(Events.EvtAddTile));
		}

		virtual protected void EvtRemoveTile(Tile tile)
		{
			if (null == broadcaster) { return; }
			broadcaster.Broadcast(new Event(Events.EvtRemoveTile));
		}

		virtual protected void EvtTileModified(Tile tile)
		{
			
		}
	}
}

//virtual PJ_BoardGrid* newBoardGrid(int width, int height, BoardDistro distro)
//{
//	return new PJ_BoardGrid(this, width, height, distro);
//}


//public:
//	bool mSuspendNotify;    // TRUE: suspend notification for add/remove events.
//PJ_Vector2Int mSize;
//GridVector mGrids;
//TileSet mTiles;
//PJ_TSelection<PJ_GridTile> mSelection;
//BoardDistro mDistro;    // OPTIMIZE: turn off distro tracking if you need more speed.

//PJ_GridBoard(BoardDistro distro)
//{
//	mDistro = distro;
//}
//PJ_GridBoard()
//:   mDistro(BoardDistro::Ignore)
//{

//}

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

//	mSize = PJ_Vector2Int(width, height);
//	mDepth = depth;

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
//bool IsValidLoc(Vector3Int loc) const {
//		if (loc.x< 0 || loc.y< 0 || loc.x >= mSize.x() || loc.y >= mSize.y()) {
//			return false;
//		}
//		if (loc.z< 0 || loc.z >= mDepth) {
//			return false;
//		}
//		return true;
//	}
	
//	virtual PJ_GridCell* NewCell() const { return new PJ_GridCell; }
	
//	PJ_GridCell* GetCell(Vector3Int loc) const {
//		if (!IsValidLoc(loc)) { return NULL; }
//		PJ_GridCell* result = NULL;

//PJ_BoardGrid* grid = mGrids[loc.z].get();
//result = grid->GetCell(loc);
//		if (NULL == result) {
//			result = NewCell();
//result->mLoc = loc;
//			grid->SetCell(loc, result);
//		}
		
//		return result;
		
//	}
	
//	bool IsCellBlocked(Vector3Int loc) const {
//		if (!IsValidLoc(loc)) { return true; }
//		return mGrids[loc.z]->IsCellBlocked(loc);
//	}
	
//	bool IsBlocked(PJ_VecRect2Int bounds, int depth) const {
//		for (int x = bounds.left(); x <= bounds.right(); x++) {
//			for (int y = bounds.top(); y <= bounds.bottom(); y++) {
//				if (IsCellBlocked(Vector3Int(x, y, depth))) {
//					return true;
//				}
//			}
//		}
		
//		return false;
//	}
	
//	PJ_GridTile* GetTile(Vector3Int loc) const {
//		PJ_GridCell* cell = GetCell(loc);
//		if (NULL != cell) {
//			return static_cast<PJ_GridTile*>(cell->mTile);
//		}
//		return NULL;
		
//	}
	
//	PJ_VecRect2Int GetDestTileBounds(PJ_GridTile* tile, Vector3Int loc)
//{

//	PJ_VecRect2Int result(loc.x, loc.y);
//	result.SetSize(tile->mSize.x(), tile->mSize.y());
//	return result;
//}

//bool PutTile(PJ_GridTile* tile, Vector3Int loc)
//{
//	if (loc.z < 0 || loc.z >= static_cast<int>(mGrids.size())) { return false; }
//	if (NULL == tile) { return false; }
//	if (tile->mSize.x() < 1 || tile->mSize.y() < 1)
//	{
//		PJLog("ERROR. Invalid tile size %d, %d", tile->mSize.x(), tile->mSize.y());
//		return false;
//	}

//	PJ_VecRect2Int tileBounds = GetDestTileBounds(tile, loc);
//	if (IsBlocked(tileBounds, loc.z))
//	{
//		return false;
//	}

//	mTiles.Put(tile);

//	tile->mBoard = this;
//	tile->mGrid = mGrids[loc.z].get();
//	tile->mOrigin = loc;

//	for (int x = tileBounds.left(); x <= tileBounds.right(); x++)
//	{
//		for (int y = tileBounds.top(); y <= tileBounds.bottom(); y++)
//		{
//			GetCell(Vector3Int(x, y, loc.z))->mTile = tile;
//		}
//	}
//	tile->mGrid->evtCellsBlocked(tileBounds);

//	// Don't notify until tile's state is set (or wrong state will be encoded).
//	if (!mSuspendNotify)
//	{
//		evtAddTile(tile);
//	}

//	return true;
//}

//void RemoveTile(PJ_GridTile* tile)
//{
//	if (NULL == tile) { return; }
//	if (tile->mBoard != this) { return; }

//	if (!mSuspendNotify)
//	{
//		evtRemoveTile(tile);    // Before release
//	}

//	Vector3Int loc = tile->mOrigin;
//	PJ_VecRect2Int tileBounds = GetDestTileBounds(tile, loc);

//	mSelection.Remove(tile);
//	tile->mBoard = NULL;

//	// Save value before release tile.
//	int depth = tile->mOrigin.z;

//	for (int x = tileBounds.left(); x <= tileBounds.right(); x++)
//	{
//		for (int y = tileBounds.top(); y <= tileBounds.bottom(); y++)
//		{
//			GetCell(Vector3Int(x, y, depth))->mTile = NULL;
//		}
//	}
//	mTiles.Remove(tile);
//	mGrids[depth]->evtCellsUnblocked(tileBounds);

//	if (!mSuspendNotify)
//	{
//		mBroadcaster->Broadcast(kEvtTileRemoved);
//	}
//}

//virtual void RemoveAllTiles()
//{
//	set<PJ_GridTile*> iterTiles = mTiles;
//	FOR_I(set<PJ_GridTile*>, iterTiles) {
//		RemoveTile(*i);
//	}
//}

//void MoveTile(Tile* tile, Vector3Int newLoc);
//bool SwapColumn(Vector3Int a, Vector3Int b);
//bool SwapRow(Vector3Int a, Vector3Int b);
//void SlideColumn(Vector3Int a, int offset, bool wrap);
//void SlideRow(Vector3Int a, int offset, bool wrap);

//virtual bool DoesAxialIndexMatchType(int index, AxialType type) const;
//virtual PJ_Vector2Int GetAxial(int index) const;
//virtual Vector3Int GridAxialToGridLoc(Vector3Int origin, PJ_Vector2Int axialOffset);
//virtual int GetNumAxial() const { return 8; }
//	virtual int GetAxialIndex(PJ_Vector2Int axial) const;
//virtual int GetNextAxialIndex(int axialIndex, AxialDir dir) const;
//virtual PJ_Vector2Int GetNextAxial(int axialIndex, AxialDir dir) const;
//virtual void CollectNeighbors(Tile* tile, vector<Tile*>& neighbors);
//virtual bool DoTilesTouch(Tile* tile1, Tile* tile2, AxialType axialType);

//virtual bool IsRowEmpty(Vector3Int row) const;
//virtual bool IsColumnEmpty(Vector3Int col) const;
//virtual bool IsRowFull(Vector3Int row) const;
//virtual bool IsColumnFull(Vector3Int col) const;

//int CountTilesInColumn(Vector3Int col) const;
//int CountTilesInRow(Vector3Int col) const;

//virtual void evtUpdate(PJ_TimeSlice const& task);

//// GO:
//int Width() const { return mSize.x(); }
//	int Height() const { return mSize.y(); }
//	int Depth() const { return mDepth; }
	
//};


///*
//	mapDistroLocSize
 
//	Maps which cells are an open slot for a tile of the specified size.
	
// */
//void PJ_BoardGrid::mapDistroLocSize(Vector3Int loc, PJ_Vector2Int size, bool testBlocked)
//{
//	if ((loc.x + size.x() - 1) >= Width() ||
//		(loc.y + size.y() - 1) >= Height())
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

//	PJ_GridCell* cell = GetCell(loc);
//	if (NULL == cell)
//	{
//		cell = new PJ_GridCell(loc);
//		SetCell(loc, cell);
//	}
//	if (!isBlocked)
//	{
//		getDistroCellIterator(size)->second.insert(cell);
//	}
//	else
//	{
//		getDistroCellIterator(size)->second.erase(cell);
//	}

//}

///*
//	evtCellsUnblocked
 
//	Called when a tile is removed, updates the open slots map.
 
// */
//void PJ_BoardGrid::evtCellsUnblocked(PJ_VecRect2Int const& blocked)
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
//	FOR_I(set<PJ_Vector2Int>, mDistroSizes) {
//		PJ_Vector2Int size = *i;

//		// FUTURE: this could be further optimized since we know that if size is smaller than the
//		// unblocked bounds, as long as size fits we don't have to test IsBlocked (good enough for now).
//		for (int x = blocked.left() - (size.x() - 1); x <= blocked.right(); x++)
//		{
//			for (int y = blocked.top() - (size.y() - 1); y <= blocked.bottom(); y++)
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
//Vector3Int PJ_BoardGrid::FindRandomLocForTile(PJ_Vector2Int tileSize)
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
//	PJ_BoardGrid::DistroCellMap::iterator cellI = getDistroCellIterator(tileSize);  // Always returns an iterator
//	size_t numCells = cellI->second.size();

//	if (numCells > 0)
//	{
//		size_t choice = PJ_Random::Choice(numCells);

//		// NOTE: possibly optimize this in the future. We need set for fast remove, but random
//		// choice isn't as efficient. (optimizing remove is more important).
//		CellSet::iterator chooseI = cellI->second.begin();
//		for (int choose = 0; choose < choice; choose++)
//		{
//			chooseI++;
//		}
//		result = (*chooseI)->mLoc;
//	}

//	return result;

//}


//void PJ_GridBoard::MoveTile(PJ_GridBoard::Tile* tile, Vector3Int newLoc)
//{
//	if (newLoc.z != tile->mOrigin.z)
//	{
//		PJLog("ERROR. MoveTile only moves within the same z grid.");
//		return;
//	}

//	// Don't notify, we're just moving the tile, not removing it.
//	PJ_TChangeAndRestore<bool> altSuspendNotify(mSuspendNotify, true);

//	Vector3Int firstLoc = tile->mOrigin;

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
//bool PJ_GridBoard::SwapColumn(Vector3Int a, Vector3Int b)
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
//		Tile* tileA = GetTile(Vector3Int(a.x, y, a.z));
//		if (NULL != tileA)
//		{
//			pjRetain(tileA);
//			RemoveTile(tileA);
//		}

//		Tile* tileB = GetTile(Vector3Int(b.x, y, b.z));
//		if (NULL != tileB)
//		{
//			Vector3Int oldLocB = tileB->mOrigin;
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

//		if (NULL != tileA)
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
//bool PJ_GridBoard::SwapRow(Vector3Int a, Vector3Int b)
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
//		Tile* tileA = GetTile(Vector3Int(x, a.y, a.z));
//		if (NULL != tileA)
//		{
//			pjRetain(tileA);
//			RemoveTile(tileA);
//		}

//		Tile* tileB = GetTile(Vector3Int(x, b.y, b.z));
//		if (NULL != tileB)
//		{
//			Vector3Int oldLocB = tileB->mOrigin;
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

//		if (NULL != tileA)
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
//void PJ_GridBoard::SlideColumn(Vector3Int a, int offset, bool wrap)
//{
//	// Don't notify, we're just moving the tiles.
//	PJ_TChangeAndRestore<bool> altSuspendNotify(mSuspendNotify, true);

//	vector<PJ_GridTile*> tiles;
//	for (int y = 0; y < Height(); y++)
//	{
//		PJ_GridTile* tile = GetTile(Vector3Int(a.x, y, a.z));
//		if (NULL == tile)
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
//		PJ_GridTile* tile = *i;
//		Vector3Int newLoc = tile->mOrigin;
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
//void PJ_GridBoard::SlideRow(Vector3Int a, int offset, bool wrap)
//{
//	// Don't notify, we're just moving the tiles.
//	PJ_TChangeAndRestore<bool> altSuspendNotify(mSuspendNotify, true);

//	vector<PJ_GridTile*> tiles;
//	for (int x = 0; x < Width(); x++)
//	{
//		PJ_GridTile* tile = GetTile(Vector3Int(x, a.y, a.z));
//		if (NULL == tile)
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
//		PJ_GridTile* tile = *i;
//		Vector3Int newLoc = tile->mOrigin;
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

//void PJ_BoardGrid::evtCellsBlocked(PJ_VecRect2Int const& blocked)
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
//	FOR_I(set<PJ_Vector2Int>, mDistroSizes) {
//		PJ_Vector2Int size = *i;
//		DistroCellMap::iterator cellIter = getDistroCellIterator(size);

//		for (int x = blocked.left() - (size.x() - 1); x <= blocked.right(); x++)
//		{
//			for (int y = blocked.top() - (size.y() - 1); y <= blocked.bottom(); y++)
//			{
//				if (!IsValidLoc(Vector3Int(x, y))) { continue; }

//# ifdef __DEBUG__
//				//				PJ_VecRect2Int thisBounds(x,y);
//				//				thisBounds.SetSize(size);
//				//				assert(thisBounds.TestIntersect(blocked));
//#endif

//				PJ_GridCell* cell = GetCell(Vector3Int(x, y));
//				cellIter->second.erase(cell);
//			}
//		}
//	}

//}

//bool PJ_BoardGrid::IsCellBlocked(Vector3Int loc) const  {
//	if (!IsValidLoc(loc)) { return true; }
//	PJ_GridCell* cell = GetCell(loc);
//	if (NULL == cell) {
//		return false;	// Nothing there.
//	}
//	if (NULL != cell->mTile && cell->mTile->mIsGhost) {
//		return false;	// Ghost tile.
//	}
//	return NULL != cell->mTile;
//}

//bool PJ_BoardGrid::IsBlocked(PJ_VecRect2Int bounds) const {
//	for (int x = bounds.left(); x <= bounds.right(); x++) {
//		for (int y = bounds.top(); y <= bounds.bottom(); y++) {
//			if (IsCellBlocked(Vector3Int(x, y))) {
//				return true;
//			}
//		}
//	}
	
//	return false;
//}

//void PJ_BoardGrid::buildMapsForSize(PJ_Vector2Int size)
//{
//	// FUTURE: support resize of the board.
//	if (mDistroSizes.find(size) != mDistroSizes.end())
//	{
//		return;
//	}
//	mDistroSizes.insert(size);  // We are now tracking slots for this tile size.
//	PJLog("Built map for size %d, %d", size.x(), size.y());

//	int width = Width();
//	int height = Height();
//	for (int x = 0; x <= (width - size.x()); x++)
//	{
//		for (int y = 0; y <= (height - size.y()); y++)
//		{
//			mapDistroLocSize(Vector3Int(x, y), size, true);
//		}
//	}

//}

///*
//	getDistroCellIterator

//	RETURNS: a set of cells that are open slots available for the specified tile size.
 
// */
//PJ_BoardGrid::DistroCellMap::iterator PJ_BoardGrid::getDistroCellIterator(PJ_Vector2Int size)
//{
//	DistroCellMap::iterator i = mDistroCellMap.find(size);
//	if (mDistroCellMap.end() == i)
//	{
//		CellSet newSet;
//		i = mDistroCellMap.insert(make_pair(size, newSet)).first;
//	}

//	return i;

//}

///*
// 	sGridNeighborAxialLocs
 
// 	ORDER: top-left, running clockwise.
 
// */
//PJ_Vector2Int PJ_GridBoard::sGridNeighborAxialLocs[] = {
//	PJ_Vector2Int(-1, -1),
//	PJ_Vector2Int(0, -1),
//	PJ_Vector2Int(1, -1),
//	PJ_Vector2Int(1, 0),
//	PJ_Vector2Int(1, 1),
//	PJ_Vector2Int(0, 1),
//	PJ_Vector2Int(-1, 1),
//	PJ_Vector2Int(-1, 0)
//};

//PJ_Vector2Int PJ_GridBoard::GetAxial(int index) const {
//	PJ_Vector2Int result;
//	if (index< 0 || index> 7) {
//		return result;
//	}
	
//	return sGridNeighborAxialLocs[index];
//}

//int PJ_GridBoard::GetAxialIndex(PJ_Vector2Int axial) const {
	
//	// FUTURE: use map for optimization if needed.
//	for (int i = 0; i<GetNumAxial(); i++) {
//		PJ_Vector2Int axialOffset = sGridNeighborAxialLocs[i];
//		if (axialOffset == axial) {
//			return i;
//		}
//	}
	
//	return -1;	// Invalid.
	
//}

//int PJ_GridBoard::GetNextAxialIndex(int axialIndex, AxialDir dir) const {
	
//	switch (dir) {
//		case AxialDir::Right:
//			axialIndex++;
//			break;
//		default:
//			axialIndex--;
//			if (axialIndex< 0) {
//				axialIndex = GetNumAxial()-1;
//			}
//			break;
//	}
	
//	axialIndex %= GetNumAxial();
	
//	return axialIndex;
//}

//PJ_Vector2Int PJ_GridBoard::GetNextAxial(int axialIndex, AxialDir dir) const {
//	int nextIndex = GetNextAxialIndex(axialIndex, dir);
//	return sGridNeighborAxialLocs[nextIndex];
	
//}

//Vector3Int PJ_GridBoard::GridAxialToGridLoc(Vector3Int origin, PJ_Vector2Int axialOffset)
//{
//	Vector3Int result = origin;
//	result.x += axialOffset.x();
//	result.y += axialOffset.y();
//	return result;

//}

//void PJ_GridBoard::CollectNeighbors(Tile* tile, vector<Tile*>& neighbors)
//{

//	neighbors.clear();

//	for (int i = 0; i < GetNumAxial(); i++)
//	{
//		PJ_Vector2Int axialOffset = sGridNeighborAxialLocs[i];
//		Vector3Int neighborLoc = GridAxialToGridLoc(tile->mOrigin, axialOffset);
//		Tile* neighbor = static_cast<Tile*>(GetTile(neighborLoc));
//		if (NULL != neighbor)
//		{
//			neighbors.push_back(neighbor);
//		}
//	}

//}

//bool PJ_GridBoard::DoTilesTouch(Tile* tile1, Tile* tile2, AxialType axialType)
//{
//	if (NULL == tile1 || NULL == tile2)
//	{
//		return false;
//	}
//	if (tile1 == tile2)
//	{
//		PJLog("ERROR. DoTilesTouch can't compare the same tile.");
//		return false;
//	}

//	bool result = false;

//	for (int i = 0; i < GetNumAxial(); i++)
//	{
//		PJ_Vector2Int axialOffset = GetAxial(i);
//		Vector3Int neighborLoc = GridAxialToGridLoc(tile1->mOrigin, axialOffset);
//		if (!DoesAxialIndexMatchType(i, axialType))
//		{
//			continue;
//		}

//		if (neighborLoc == tile2->mOrigin)
//		{
//			return true;
//		}
//	}

//	return result;

//}

//bool PJ_GridBoard::DoesAxialIndexMatchType(int index, AxialType type) const {
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

//bool PJ_GridBoard::IsRowEmpty(Vector3Int row) const {
//	for (int x = 0; x<Width(); x++) {
//		Vector3Int loc = row;
//loc.x = x;
//		if (NULL != GetTile(loc)) {
//			return false;
//		}
//	}
//	return true;
//}

//bool PJ_GridBoard::IsColumnEmpty(Vector3Int col) const {
//	for (int y = 0; y<Height(); y++) {
//		Vector3Int loc = col;
//loc.y = y;
//		if (NULL != GetTile(loc)) {
//			return false;
//		}
//	}
//	return true;

//}

//bool PJ_GridBoard::IsRowFull(Vector3Int row) const {
//	for (int x = 0; x<Width(); x++) {
//		Vector3Int loc = row;
//loc.x = x;
//		if (NULL == GetTile(loc)) {
//			return false;
//		}
//	}
//	return true;
//}

//bool PJ_GridBoard::IsColumnFull(Vector3Int col) const {
//	for (int y = 0; y<Height(); y++) {
//		Vector3Int loc = col;
//loc.y = y;
//		if (NULL == GetTile(loc)) {
//			return false;
//		}
//	}
//	return true;
	
//}

//int PJ_GridBoard::CountTilesInColumn(Vector3Int col) const {
	
//	int result = 0;
//	for (int y = 0; y<Height(); y++) {
//		Tile* tile = GetTile(Vector3Int(col.x, y));
//		if (NULL == tile) {
//			continue;
//		}
//		result++;
//		y += tile->mSize.y()-1;
		
//	}
	
//	return result;
	
//}

//int PJ_GridBoard::CountTilesInRow(Vector3Int col) const {
	
//	int result = 0;
//	for (int x = 0; x<Width(); x++) {
//		Tile* tile = GetTile(Vector3Int(x, col.y));
//		if (NULL == tile) {
//			continue;
//		}
//		result++;
//		x += tile->mSize.x()-1;
//	}
	
//	return result;
	
//}

//void PJ_GridBoard::evtUpdate(PJ_TimeSlice const& task)
//{

//	// Avoid mutation error if tile set changes during update.
//	set<PJ_GridTile*> iterTiles = mTiles;
//	FOR_CONST_I(TileSet, iterTiles) {
//		(*i)->evtUpdate(task);
//	}

//}

//#pragma mark - PJ_GridTile

//void PJ_GridTile::evtModified()
//{
//	if (NULL != mBoard) mBoard->evtTileModified(this);
//}
