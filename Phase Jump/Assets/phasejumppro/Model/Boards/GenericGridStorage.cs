using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace PJ
{
	/// <summary>
	/// Stores objects in an XxY grid structure (Y rows of X cells)
	/// </summary>
	/// <typeparam name="T">Type of thing to store</typeparam>
	public class GenericGridStorage<T>
	{
		#region Types
		public class Row : List<T>
		{
			public Row(int capacity)
			: base(capacity)
			{
			}
		}
		#endregion

		#region Fields
		protected List<Row> rows;
		protected Vector2Int size;
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

		public GenericGridStorage()
		{
		}

		public GenericGridStorage(Vector2Int size)
		{
			if (size.x < 1 || size.y < 1)
			{
				Debug.LogError(string.Format("Invalid grid storage size {0}, {1}.", size.x, size.y));
				return;
			}

			this.size = size;

			for (int i = 0; i < size.y; i++)
			{
				Row row = new Row(size.x);
				rows.Add(row);
			}
		}
	}
}

// TODO: port this

//protected:
//	virtual void removeCells(Row row, CellIterator first, CellIterator end)
//{
//	row->erase(first, end);
//}

//virtual void RemoveRows(RowIterator first, RowIterator end)
//{

//	for (typename PJ_TStdVector< Row *>.iterator i = first; i != end; i++) {
//		Row row = (*i);
//		delete row;

//	}

//	rows.erase(first, end);

//}

//void Resize(int width, int height, T defaultVal)
//{

//# ifdef __DEBUG__
//	if (width < 1 || height < 1)
//	{
//		PJLog("ERROR. Invalid grid size %d, %d", width, height);
//		return;
//	}
//#endif

//	// Same size.
//	if (width == mSize[0] && height == mSize[1]) { return; }

//	// Change width of existing rows first.
//	int y = 0;
//	for (typename PJ_TStdVector< Row *>.iterator i = rows.begin(); i != rows.end(); i++, y++) {

//		// Don't bother resizing rows that are going to be deleted if the
//		// grid is shrinking.
//		if (y >= height)
//		{
//			break;
//		}

//		Row row = (*i);
//		int sizeX = mSize[0];

//		while (sizeX < width)
//		{
//			row->Add(defaultVal);
//			sizeX++;
//		}
//		if (sizeX > width)
//		{
//			int diff = sizeX - width;

//			typename Row.iterator  first = row->end();
//			first -= diff;
//			removeCells(row, first, row->end());

//			sizeX = width;
//		}
//	}

//	if (mSize[1] > height)
//	{
//		int diff = mSize[1] - height;

//		typename PJ_TStdVector<Row*>.iterator  first = rows.end() - diff;
//		RemoveRows(first, rows.end());
//		mSize[1] = height;
//	}
//	else
//	{
//		while (mSize[1] < height)
//		{
//			Row row = new Row(width, defaultVal);
//			rows.Add(row);
//			mSize[1]++;
//		}
//	}

//	mSize[0] = width;
//	mSize[1] = height;

//}

//inline Row GetRow(typename PJ_TStdVector<Row*>.size_type y) const { return y<rows.size()? rows[y] : NULL; }
	
//	bool IsValidLoc(PJ_GridLoc const& loc) const {
//		return (loc.x >= 0 && loc.x<mSize[0] &&
//				loc.y >= 0 && loc.y<mSize[1]);
//	}
	
//	virtual T GetCell(PJ_GridLoc loc) const {
//		if (loc.x >= 0 && loc.x<mSize[0] &&
//			loc.y >= 0 && loc.y<mSize[1]) {
			
//			Row row = rows[loc.y];
//			return ( row)[loc.x];
//		}
		
//		// http://stackoverflow.com/questions/936999/what-is-the-default-constructor-for-c-pointer
//		return T();
		
//	}
	
//	virtual void SetCell(PJ_GridLoc loc, T& content)
//{
//	if (loc.x >= 0 && loc.x < mSize[0] &&
//		loc.y >= 0 && loc.y < mSize[1])
//	{

//		Row row = GetRow(loc.y);
//		if (NULL != row)
//		{
//			(*row)[loc.x] = content;
//		}
//	}
//}

//typename Row.iterator IteratorOf(PJ_GridLoc const& loc)
//{
//	if (loc.x >= mSize[0] || loc.x < 0) { return rows.end(); }
//	if (loc.y >= mSize[1] || loc.y < 0) { return rows.end(); }

//	Row row = rows[loc.y];
//	return row->begin() + loc.x;

//}
	
//};
