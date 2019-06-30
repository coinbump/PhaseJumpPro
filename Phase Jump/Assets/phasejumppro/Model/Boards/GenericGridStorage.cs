using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace PJ
{
	/// <summary>
	/// Stores objects in an XxY grid structure (Y rows of X cells)
	/// </summary>
	/// <typeparam name="T">Type of thing to store</typeparam>
	public class GenericGridStorage<T> where T : new()
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

		#region Initializers
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
		#endregion

		Row GetRow(int index) { return index < rows.Count ? rows[index] : null; }

		void Resize(Vector2Int newSize)
		{
			var width = newSize.x;
			var height = newSize.y;

			if (width < 1 || height < 1)
			{
				Debug.LogError(string.Format("Invalid grid storage size {0}, {1}.", size.x, size.y));
				return;
			}

			// Same size.
			if (width == Width && height == Height) { return; }

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
			//		int sizeX = Width;

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

			//	if (Height > height)
			//	{
			//		int diff = Height - height;

			//		typename PJ_TStdVector<Row*>.iterator  first = rows.end() - diff;
			//		RemoveRows(first, rows.end());
			//		Height = height;
			//	}
			//	else
			//	{
			//		while (Height < height)
			//		{
			//			Row row = new Row(width, defaultVal);
			//			rows.Add(row);
			//			Height++;
			//		}
			//	}

			//	Width = width;
			//	Height = height;

		}

		public bool IsValidLoc(Vector2Int loc) {
			return (loc.x >= 0 && loc.x < Width &&
					loc.y >= 0 && loc.y < Height);
		}

		public virtual T GetCell(Vector2Int loc)
		{
			if (loc.x >= 0 && loc.x < Width &&
				loc.y >= 0 && loc.y < Height)
			{
				Row row = rows[loc.y];
				return (row)[loc.x];
			}

			return new T();
		}

		public virtual void SetCell(Vector2Int loc, T content)
		{
			if (loc.x >= 0 && loc.x < Width &&
				loc.y >= 0 && loc.y < Height)
			{
				Row row = GetRow(loc.y);
				if (null != row)
				{
					(row)[loc.x] = content;
				}
			}
		}
	}
}
