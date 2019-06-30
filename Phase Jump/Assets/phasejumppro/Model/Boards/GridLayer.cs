using UnityEngine;
using System.Collections;

namespace PJ
{
	class GridLayer
	{

	}
}

// TODO: port this C++ code to C#
///*
//	GridLayer

//	Stores model data for each layer of the grid board.

// */
//class GridLayer : public PJ_TPtrGrid<PJ_GridCell> {
//private:
//	typedef PJ_TPtrGrid<PJ_GridCell>    Super;

//public:
//	typedef set<PJ_GridCell*>       CellSet;

//BoardDistro mDistro;    // OPTIMIZE: turn off distro tracking if you need more speed.

//protected:
//	// DISTRIBUTION INFO:
//	typedef map<PJ_Vector2Int, CellSet> DistroCellMap;
//DistroCellMap mDistroCellMap;
//set<PJ_Vector2Int> mDistroSizes;    // Sizes that have been mapped for distribution

//protected:
//	DistroCellMap::iterator getDistroCellIterator(PJ_Vector2Int size);
//void buildMapsForSize(PJ_Vector2Int size);
//void mapDistroLocSize(PJ_GridLoc loc, PJ_Vector2Int size, bool testBlocked);

//public:
//	PJ_GridBoard* mOwner;

//GridLayer(PJ_GridBoard* owner, int width, int height, BoardDistro distro);
//virtual ~GridLayer();

//PJ_GridLoc FindRandomLocForTile(PJ_Vector2Int tileSize);
//virtual void evtCellsBlocked(PJ_VecRect2Int const& blocked);
//virtual void evtCellsUnblocked(PJ_VecRect2Int const& blocked);

//bool IsCellBlocked(PJ_GridLoc loc) const;
//bool IsBlocked(PJ_VecRect2Int bounds) const;


/*
	mapDistroLocSize
 
	Maps which cells are an open slot for a tile of the specified size.
	
 */
//void PJ_BoardGrid::mapDistroLocSize(PJ_GridLoc loc, PJ_Vector2Int size, bool testBlocked)
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
//				if (!IsValidLoc(PJ_GridLoc(x, y))) { continue; }

//# ifdef __DEBUG__
//				//				PJ_VecRect2Int	thisBounds(x,y);
//				//				thisBounds.SetSize(size);
//				//				assert(thisBounds.TestIntersect(blocked));
//#endif

//				mapDistroLocSize(PJ_GridLoc(x, y), size, true);
//			}
//		}
//	}

//}

///*
//	FindRandomLocForTile
 
//	If tracking.
 
// */
//PJ_GridLoc PJ_BoardGrid::FindRandomLocForTile(PJ_Vector2Int tileSize)
//{
//	PJ_GridLoc result(-1, -1);  // Invalid.
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

//PJ_BoardGrid::PJ_BoardGrid(PJ_GridBoard* owner, int width, int height, BoardDistro distro)
//:	PJ_TPtrGrid<PJ_GridCell>(width, height),

//	mOwner(owner)
//{

//	mDistro = distro;
//}

//PJ_BoardGrid::~PJ_BoardGrid()
//{
//# ifdef __DEBUG__
//	int breakpoint = 0; breakpoint++;
//#endif

//}

//void PJ_GridBoard::MoveTile(PJ_GridBoard::Tile* tile, PJ_GridLoc newLoc)
//{
//	if (newLoc.z != tile->mOrigin.z)
//	{
//		PJLog("ERROR. MoveTile only moves within the same z grid.");
//		return;
//	}

//	// Don't notify, we're just moving the tile, not removing it.
//	PJ_TChangeAndRestore<bool> altSuspendNotify(mSuspendNotify, true);

//	PJ_GridLoc firstLoc = tile->mOrigin;

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
//bool PJ_GridBoard::SwapColumn(PJ_GridLoc a, PJ_GridLoc b)
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
//		Tile* tileA = GetTile(PJ_GridLoc(a.x, y, a.z));
//		if (NULL != tileA)
//		{
//			pjRetain(tileA);
//			RemoveTile(tileA);
//		}

//		Tile* tileB = GetTile(PJ_GridLoc(b.x, y, b.z));
//		if (NULL != tileB)
//		{
//			PJ_GridLoc oldLocB = tileB->mOrigin;
//			pjRetain(tileB);
//			RemoveTile(tileB);
//			PJ_GridLoc newLoc(a.x, y, a.z);
//			if (!PutTile(tileB, newLoc))
//			{
//				PJLog("ERROR. SwapColumn didn't fit at %d, %d, %d.", newLoc.x, newLoc.y, newLoc.z);
//				PutTile(tileB, oldLocB);
//				return false;
//			}
//		}

//		if (NULL != tileA)
//		{
//			PJ_GridLoc newLoc(b.x, y, b.z);
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
//bool PJ_GridBoard::SwapRow(PJ_GridLoc a, PJ_GridLoc b)
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
//		Tile* tileA = GetTile(PJ_GridLoc(x, a.y, a.z));
//		if (NULL != tileA)
//		{
//			pjRetain(tileA);
//			RemoveTile(tileA);
//		}

//		Tile* tileB = GetTile(PJ_GridLoc(x, b.y, b.z));
//		if (NULL != tileB)
//		{
//			PJ_GridLoc oldLocB = tileB->mOrigin;
//			pjRetain(tileB);
//			RemoveTile(tileB);
//			PJ_GridLoc newLoc(x, a.y, a.z);
//			if (!PutTile(tileB, newLoc))
//			{
//				PJLog("ERROR. SwapRow didn't fit at %d, %d, %d.", newLoc.x, newLoc.y, newLoc.z);
//				PutTile(tileB, oldLocB);
//				return false;
//			}
//		}

//		if (NULL != tileA)
//		{
//			PJ_GridLoc newLoc(x, b.y, b.z);
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
//void PJ_GridBoard::SlideColumn(PJ_GridLoc a, int offset, bool wrap)
//{
//	// Don't notify, we're just moving the tiles.
//	PJ_TChangeAndRestore<bool> altSuspendNotify(mSuspendNotify, true);

//	vector<PJ_GridTile*> tiles;
//	for (int y = 0; y < Height(); y++)
//	{
//		PJ_GridTile* tile = GetTile(PJ_GridLoc(a.x, y, a.z));
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
//		PJ_GridLoc newLoc = tile->mOrigin;
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
//void PJ_GridBoard::SlideRow(PJ_GridLoc a, int offset, bool wrap)
//{
//	// Don't notify, we're just moving the tiles.
//	PJ_TChangeAndRestore<bool> altSuspendNotify(mSuspendNotify, true);

//	vector<PJ_GridTile*> tiles;
//	for (int x = 0; x < Width(); x++)
//	{
//		PJ_GridTile* tile = GetTile(PJ_GridLoc(x, a.y, a.z));
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
//		PJ_GridLoc newLoc = tile->mOrigin;
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
//				if (!IsValidLoc(PJ_GridLoc(x, y))) { continue; }

//# ifdef __DEBUG__
//				//				PJ_VecRect2Int thisBounds(x,y);
//				//				thisBounds.SetSize(size);
//				//				assert(thisBounds.TestIntersect(blocked));
//#endif

//				PJ_GridCell* cell = GetCell(PJ_GridLoc(x, y));
//				cellIter->second.erase(cell);
//			}
//		}
//	}

//}

//bool PJ_BoardGrid::IsCellBlocked(PJ_GridLoc loc) const  {
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
//			if (IsCellBlocked(PJ_GridLoc(x, y))) {
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
//			mapDistroLocSize(PJ_GridLoc(x, y), size, true);
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

//PJ_GridLoc PJ_GridBoard::GridAxialToGridLoc(PJ_GridLoc origin, PJ_Vector2Int axialOffset)
//{
//	PJ_GridLoc result = origin;
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
//		PJ_GridLoc neighborLoc = GridAxialToGridLoc(tile->mOrigin, axialOffset);
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
//		PJ_GridLoc neighborLoc = GridAxialToGridLoc(tile1->mOrigin, axialOffset);
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

//bool PJ_GridBoard::IsRowEmpty(PJ_GridLoc row) const {
//	for (int x = 0; x<Width(); x++) {
//		PJ_GridLoc loc = row;
//loc.x = x;
//		if (NULL != GetTile(loc)) {
//			return false;
//		}
//	}
//	return true;
//}

//bool PJ_GridBoard::IsColumnEmpty(PJ_GridLoc col) const {
//	for (int y = 0; y<Height(); y++) {
//		PJ_GridLoc loc = col;
//loc.y = y;
//		if (NULL != GetTile(loc)) {
//			return false;
//		}
//	}
//	return true;

//}

//bool PJ_GridBoard::IsRowFull(PJ_GridLoc row) const {
//	for (int x = 0; x<Width(); x++) {
//		PJ_GridLoc loc = row;
//loc.x = x;
//		if (NULL == GetTile(loc)) {
//			return false;
//		}
//	}
//	return true;
//}

//bool PJ_GridBoard::IsColumnFull(PJ_GridLoc col) const {
//	for (int y = 0; y<Height(); y++) {
//		PJ_GridLoc loc = col;
//loc.y = y;
//		if (NULL == GetTile(loc)) {
//			return false;
//		}
//	}
//	return true;
	
//}

//int PJ_GridBoard::CountTilesInColumn(PJ_GridLoc col) const {
	
//	int result = 0;
//	for (int y = 0; y<Height(); y++) {
//		Tile* tile = GetTile(PJ_GridLoc(col.x, y));
//		if (NULL == tile) {
//			continue;
//		}
//		result++;
//		y += tile->mSize.y()-1;
		
//	}
	
//	return result;
	
//}

//int PJ_GridBoard::CountTilesInRow(PJ_GridLoc col) const {
	
//	int result = 0;
//	for (int x = 0; x<Width(); x++) {
//		Tile* tile = GetTile(PJ_GridLoc(x, col.y));
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

//#pragma mark - UNIT TESTS

//# ifdef __UNIT_TESTS__

//# include "gtest.h"

//class TestBoardGrid : public PJ_BoardGrid {
//public:
//	PJ_VecRect2Int mLastCellsBlocked;
//PJ_VecRect2Int mLastCellsUnblocked;

//TestBoardGrid(PJ_GridBoard* owner, int width, int height, BoardDistro distro)
//:   PJ_BoardGrid(owner, width, height, distro)
//{
//}

//// OVERRIDE:
//virtual void evtCellsBlocked(PJ_VecRect2Int const& blocked)
//{
//	mLastCellsBlocked = blocked;
//	PJ_BoardGrid::evtCellsBlocked(blocked);
//}
//virtual void evtCellsUnblocked(PJ_VecRect2Int const& blocked)
//{
//	mLastCellsUnblocked = blocked;
//	PJ_BoardGrid::evtCellsUnblocked(blocked);
//}

//};

//class TestGridBoard : public PJ_GridBoard {
//protected:
//	// OVERRIDE:
//	virtual PJ_BoardGrid* newBoardGrid(int width, int height, BoardDistro distro)
//{
//	return new TestBoardGrid(this, width, height, distro);
//}

//public:
//	int mDeconstruct;

//TestGridBoard(BoardDistro distro)
//:   PJ_GridBoard(distro)
//{
//}

//void FillWith1by1();
//};

//class TestGridTile : public PJ_GridTile {
//public:
//	int* mDeconstruct;
//PJ_GridLoc mOldLoc;
//PJ_String mObject;

//TestGridTile(int* deconstruct)
//:   PJ_GridTile(PJ_Vector2Int(2,2))
//	{
//		* deconstruct = 0;
//mDeconstruct = deconstruct;
//	}
//	virtual ~TestGridTile()
//{
//	*mDeconstruct = 1;
//}
	
//};

//void TestGridBoard::FillWith1by1()
//{
//	for (int x = 0; x < this->Width(); x++)
//	{
//		for (int y = 0; y < this->Height(); y++)
//		{
//			TestGridTile* tile = new TestGridTile(&mDeconstruct);
//			tile->mSize = PJ_Vector2Int(1, 1);
//			this->PutTile(tile, PJ_GridLoc(x, y));
//			tile->mOldLoc = tile->mOrigin;
//		}
//	}

//}

//TEST(PJ_BoardGrid, Distro)
//{

//	PTR(TestGridBoard)  gridBoard(new TestGridBoard(BoardDistro::Track));
//	gridBoard->Build(20, 20, 2);
//	EXPECT_EQ(2, gridBoard->Depth());
//	EXPECT_EQ(20, gridBoard->Width());
//	EXPECT_EQ(20, gridBoard->Height());

//	int deconstruct;
//	TestGridTile* tile = new TestGridTile(&deconstruct);
//	EXPECT_TRUE(gridBoard->PutTile(tile, PJ_GridLoc(0, 0)));
//	EXPECT_EQ(static_cast<TestBoardGrid*>(gridBoard->mGrids[0].get())->mLastCellsBlocked, PJ_VecRect2Int(0, 0, 1, 1));
//	gridBoard->RemoveTile(tile);
//	EXPECT_EQ(static_cast<TestBoardGrid*>(gridBoard->mGrids[0].get())->mLastCellsUnblocked, PJ_VecRect2Int(0, 0, 1, 1));

//	tile = new TestGridTile(&deconstruct);
//	EXPECT_TRUE(gridBoard->PutTile(tile, PJ_GridLoc(1, 1)));
//	gridBoard->RemoveAllTiles();
//	EXPECT_EQ(static_cast<TestBoardGrid*>(gridBoard->mGrids[0].get())->mLastCellsUnblocked, PJ_VecRect2Int(1, 1, 2, 2));

//}

//// TODO: add unit tests.
//TEST(PJ_GridBoard, SwapAndSlide)
//{

//	{
//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
//		gridBoard->Build(5, 5, 2);
//		gridBoard->FillWith1by1();

//		EXPECT_FALSE(gridBoard->SwapColumn(PJ_GridLoc(0, 0, 0), PJ_GridLoc(0, 0, 0)));
//		EXPECT_FALSE(gridBoard->SwapColumn(PJ_GridLoc(0, 0, 0), PJ_GridLoc(2, 0, 1)));
//		EXPECT_TRUE(gridBoard->SwapColumn(PJ_GridLoc(0, 0, 0), PJ_GridLoc(2, 0, 0)));

//		for (int y = 0; y < gridBoard->Height(); y++)
//		{
//			EXPECT_EQ(PJ_GridLoc(2, y), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(0, y)))->mOldLoc);
//			EXPECT_EQ(PJ_GridLoc(0, y), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(2, y)))->mOldLoc);
//		}
//	}

//	{
//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
//		gridBoard->Build(5, 5, 2);
//		gridBoard->FillWith1by1();

//		EXPECT_FALSE(gridBoard->SwapRow(PJ_GridLoc(0, 0, 0), PJ_GridLoc(0, 0, 0)));
//		EXPECT_FALSE(gridBoard->SwapRow(PJ_GridLoc(0, 0, 0), PJ_GridLoc(0, 2, 1)));
//		EXPECT_TRUE(gridBoard->SwapRow(PJ_GridLoc(0, 0, 0), PJ_GridLoc(0, 2, 0)));

//		for (int x = 0; x < gridBoard->Width(); x++)
//		{
//			EXPECT_EQ(PJ_GridLoc(x, 2), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(x, 0)))->mOldLoc);
//			EXPECT_EQ(PJ_GridLoc(x, 0), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(x, 2)))->mOldLoc);
//		}
//	}

//	// SlideColumn: WRAP
//	{
//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
//		gridBoard->Build(5, 5, 2);
//		gridBoard->FillWith1by1();

//		gridBoard->SlideColumn(PJ_GridLoc(0, 0, 0), 3, true);
//		EXPECT_EQ(PJ_GridLoc(0, 2), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(0, 0)))->mOldLoc);
//		EXPECT_EQ(PJ_GridLoc(0, 3), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(0, 1)))->mOldLoc);
//		EXPECT_EQ(PJ_GridLoc(0, 4), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(0, 2)))->mOldLoc);
//		EXPECT_EQ(PJ_GridLoc(0, 0), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(0, 3)))->mOldLoc);
//		EXPECT_EQ(PJ_GridLoc(0, 1), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(0, 4)))->mOldLoc);
//	}

//	// SlideColumn: WRAP-NEGATIVE
//	{
//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
//		gridBoard->Build(5, 5, 2);
//		gridBoard->FillWith1by1();

//		gridBoard->SlideColumn(PJ_GridLoc(0, 0, 0), -2, true);
//		EXPECT_EQ(PJ_GridLoc(0, 2), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(0, 0)))->mOldLoc);
//		EXPECT_EQ(PJ_GridLoc(0, 3), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(0, 1)))->mOldLoc);
//		EXPECT_EQ(PJ_GridLoc(0, 4), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(0, 2)))->mOldLoc);
//		EXPECT_EQ(PJ_GridLoc(0, 0), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(0, 3)))->mOldLoc);
//		EXPECT_EQ(PJ_GridLoc(0, 1), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(0, 4)))->mOldLoc);
//	}

//	// SlideColumn: NO WRAP
//	{
//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
//		gridBoard->Build(5, 5, 2);
//		gridBoard->FillWith1by1();

//		gridBoard->SlideColumn(PJ_GridLoc(0, 0, 0), 3, false);
//		EXPECT_EQ(NULL, static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(0, 0))));
//		EXPECT_EQ(NULL, static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(0, 1))));
//		EXPECT_EQ(NULL, static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(0, 2))));
//		EXPECT_EQ(PJ_GridLoc(0, 0), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(0, 3)))->mOldLoc);
//		EXPECT_EQ(PJ_GridLoc(0, 1), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(0, 4)))->mOldLoc);
//	}

//	// SlideRow: WRAP
//	{
//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
//		gridBoard->Build(5, 5, 2);
//		gridBoard->FillWith1by1();

//		gridBoard->SlideRow(PJ_GridLoc(0, 0, 0), 3, true);
//		EXPECT_EQ(PJ_GridLoc(2, 0), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(0, 0)))->mOldLoc);
//		EXPECT_EQ(PJ_GridLoc(3, 0), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(1, 0)))->mOldLoc);
//		EXPECT_EQ(PJ_GridLoc(4, 0), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(2, 0)))->mOldLoc);
//		EXPECT_EQ(PJ_GridLoc(0, 0), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(3, 0)))->mOldLoc);
//		EXPECT_EQ(PJ_GridLoc(1, 0), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(4, 0)))->mOldLoc);
//	}

//	// SlideRow: WRAP-NEGATIVE
//	{
//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
//		gridBoard->Build(5, 5, 2);
//		gridBoard->FillWith1by1();

//		gridBoard->SlideRow(PJ_GridLoc(0, 0, 0), -2, true);
//		EXPECT_EQ(PJ_GridLoc(2, 0), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(0, 0)))->mOldLoc);
//		EXPECT_EQ(PJ_GridLoc(3, 0), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(1, 0)))->mOldLoc);
//		EXPECT_EQ(PJ_GridLoc(4, 0), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(2, 0)))->mOldLoc);
//		EXPECT_EQ(PJ_GridLoc(0, 0), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(3, 0)))->mOldLoc);
//		EXPECT_EQ(PJ_GridLoc(1, 0), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(4, 0)))->mOldLoc);
//	}

//	// SlideRow: NO WRAP
//	{
//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
//		gridBoard->Build(5, 5, 2);
//		int deconstruct = 0;
//		for (int x = 0; x < gridBoard->Width(); x++)
//		{
//			for (int y = 0; y < gridBoard->Height(); y++)
//			{
//				TestGridTile* tile = new TestGridTile(&deconstruct);
//				tile->mSize = PJ_Vector2Int(1, 1);
//				gridBoard->PutTile(tile, PJ_GridLoc(x, y));
//				tile->mOldLoc = tile->mOrigin;
//			}
//		}

//		gridBoard->SlideRow(PJ_GridLoc(0, 0, 0), 3, false);
//		EXPECT_EQ(NULL, static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(0, 0))));
//		EXPECT_EQ(NULL, static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(1, 0))));
//		EXPECT_EQ(NULL, static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(2, 0))));
//		EXPECT_EQ(PJ_GridLoc(0, 0), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(3, 0)))->mOldLoc);
//		EXPECT_EQ(PJ_GridLoc(1, 0), static_cast<TestGridTile*>(gridBoard->GetTile(PJ_GridLoc(4, 0)))->mOldLoc);
//	}

//	// TODO: test no wrap, slide row, slide row-noWRAP

//}

//TEST(PJ_GridBoard, Unit_Tests)
//{

//	PTR(TestGridBoard)  gridBoard(new TestGridBoard(BoardDistro::Track));
//	gridBoard->Build(20, 20, 2);
//	EXPECT_EQ(2, gridBoard->Depth());
//	EXPECT_EQ(20, gridBoard->Width());
//	EXPECT_EQ(20, gridBoard->Height());
//	EXPECT_FALSE(gridBoard->IsValidLoc(PJ_GridLoc(0, 0, 2)));
//	EXPECT_FALSE(gridBoard->IsValidLoc(PJ_GridLoc(0, 0, -1)));
//	EXPECT_FALSE(gridBoard->IsValidLoc(PJ_GridLoc(20, 20)));
//	EXPECT_FALSE(gridBoard->IsValidLoc(PJ_GridLoc(-1, -1)));
//	EXPECT_TRUE(gridBoard->IsValidLoc(PJ_GridLoc(19, 19)));
//	EXPECT_TRUE(gridBoard->IsValidLoc(PJ_GridLoc(0, 0)));

//	PJ_GridCell* gridCell = gridBoard->GetCell(PJ_GridLoc(0, 0));
//	EXPECT_EQ(NULL, gridCell->mTile);
//	EXPECT_EQ(NULL, gridBoard->GetTile(PJ_GridLoc(0, 0)));

//	int deconstruct;
//	TestGridTile* tile = new TestGridTile(&deconstruct);
//	EXPECT_FALSE(gridBoard->PutTile(tile, PJ_GridLoc(20, 20)));
//	EXPECT_FALSE(gridBoard->PutTile(tile, PJ_GridLoc(0, 0, 2)));
//	EXPECT_TRUE(gridBoard->PutTile(tile, PJ_GridLoc(0, 0)));

//	gridCell = gridBoard->GetCell(PJ_GridLoc(0, 0));
//	EXPECT_EQ(tile, gridCell->mTile);
//	EXPECT_EQ(tile, gridBoard->GetTile(PJ_GridLoc(0, 0)));
//	EXPECT_EQ(tile, gridBoard->GetTile(PJ_GridLoc(0, 1)));
//	EXPECT_EQ(tile, gridBoard->GetTile(PJ_GridLoc(1, 0)));
//	EXPECT_EQ(tile, gridBoard->GetTile(PJ_GridLoc(1, 1)));
//	EXPECT_EQ(NULL, gridBoard->GetTile(PJ_GridLoc(0, 0, 1)));
//	EXPECT_EQ(NULL, gridBoard->GetTile(PJ_GridLoc(2, 0)));
//	EXPECT_EQ(NULL, gridBoard->GetTile(PJ_GridLoc(0, 2)));

//	EXPECT_TRUE(gridBoard->IsCellBlocked(PJ_GridLoc(-1, -1)));
//	EXPECT_TRUE(gridBoard->IsCellBlocked(PJ_GridLoc(0, 0)));
//	EXPECT_TRUE(gridBoard->IsCellBlocked(PJ_GridLoc(0, 1)));
//	EXPECT_TRUE(gridBoard->IsCellBlocked(PJ_GridLoc(1, 0)));
//	EXPECT_TRUE(gridBoard->IsCellBlocked(PJ_GridLoc(1, 1)));
//	EXPECT_FALSE(gridBoard->IsCellBlocked(PJ_GridLoc(0, 0, 1)));
//	EXPECT_FALSE(gridBoard->IsCellBlocked(PJ_GridLoc(0, 2)));
//	EXPECT_FALSE(gridBoard->IsCellBlocked(PJ_GridLoc(2, 0)));

//	EXPECT_TRUE(gridBoard->IsBlocked(PJ_VecRect2Int(0, 0, 1, 1), 0));
//	EXPECT_TRUE(gridBoard->IsBlocked(PJ_VecRect2Int(1, 0, 2, 1), 0));
//	EXPECT_TRUE(gridBoard->IsBlocked(PJ_VecRect2Int(0, 1, 1, 2), 0));
//	EXPECT_FALSE(gridBoard->IsBlocked(PJ_VecRect2Int(2, 0, 3, 1), 0));
//	EXPECT_FALSE(gridBoard->IsBlocked(PJ_VecRect2Int(0, 2, 1, 3), 0));

//	PJ_VecRect2Int destTileBounds = gridBoard->GetDestTileBounds(tile, PJ_GridLoc(0, 0));
//	EXPECT_EQ(0, destTileBounds.left());
//	EXPECT_EQ(0, destTileBounds.top());
//	EXPECT_EQ(1, destTileBounds.right());
//	EXPECT_EQ(1, destTileBounds.bottom());

//	EXPECT_EQ(0, deconstruct);
//	gridBoard->RemoveTile(tile); tile = NULL;
//	EXPECT_EQ(1, deconstruct);
//	EXPECT_FALSE(gridBoard->IsCellBlocked(PJ_GridLoc(0, 0)));
//	EXPECT_FALSE(gridBoard->IsCellBlocked(PJ_GridLoc(0, 1)));
//	EXPECT_FALSE(gridBoard->IsCellBlocked(PJ_GridLoc(1, 0)));
//	EXPECT_FALSE(gridBoard->IsCellBlocked(PJ_GridLoc(1, 1)));
//	EXPECT_FALSE(gridBoard->IsBlocked(PJ_VecRect2Int(0, 0, 1, 1), 0));
//	EXPECT_FALSE(gridBoard->IsBlocked(PJ_VecRect2Int(1, 0, 2, 1), 0));
//	EXPECT_FALSE(gridBoard->IsBlocked(PJ_VecRect2Int(0, 1, 1, 2), 0));

//	tile = new TestGridTile(&deconstruct);
//	EXPECT_TRUE(gridBoard->PutTile(tile, PJ_GridLoc(0, 0)));
//	EXPECT_EQ(0, deconstruct);
//	gridBoard->RemoveAllTiles(); tile = NULL;
//	EXPECT_EQ(1, deconstruct);

//}

//// TODO: add unit tests for all axial methods.
//TEST(PJ_GridBoard, Test_Axial)
//{

//	PTR(PJ_GridBoard) board(new PJ_GridBoard(BoardDistro::Ignore));
//	EXPECT_EQ(1, board->GetNextAxialIndex(0, AxialDir::Right));
//	EXPECT_EQ(2, board->GetNextAxialIndex(1, AxialDir::Right));
//	EXPECT_EQ(3, board->GetNextAxialIndex(2, AxialDir::Right));
//	EXPECT_EQ(4, board->GetNextAxialIndex(3, AxialDir::Right));
//	EXPECT_EQ(5, board->GetNextAxialIndex(4, AxialDir::Right));
//	EXPECT_EQ(0, board->GetNextAxialIndex(7, AxialDir::Right));

//	EXPECT_EQ(7, board->GetNextAxialIndex(0, AxialDir::Left));
//	EXPECT_EQ(0, board->GetNextAxialIndex(1, AxialDir::Left));
//	EXPECT_EQ(1, board->GetNextAxialIndex(2, AxialDir::Left));
//	EXPECT_EQ(2, board->GetNextAxialIndex(3, AxialDir::Left));
//	EXPECT_EQ(3, board->GetNextAxialIndex(4, AxialDir::Left));
//	EXPECT_EQ(4, board->GetNextAxialIndex(5, AxialDir::Left));

//}