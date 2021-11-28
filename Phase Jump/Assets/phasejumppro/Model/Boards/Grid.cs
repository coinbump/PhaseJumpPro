using UnityEngine;
using System.Collections;
using System.Collections.Generic;

/*
 * RATING: 5 stars. Simple storage mechanism with Unit Tests
 * CODE REVIEW: 6.30.19
 */
namespace PJ
{
	/// <summary>
	/// Stores objects in an X,Y grid structure (Y rows of X cells)
	/// </summary>
	/// <typeparam name="T">Type of thing to store</typeparam>
	public class Grid<T> where T : new()
	{
		public class Row : List<T>
		{
			public Row(int capacity)
			: base(capacity)
			{
				// Fill the row (initializing with capacity doesn't do this)
				while (Count < capacity)
				{
					Add(new T());
				}
			}
		}

		protected List<Row> rows;
		protected Vector2Int size;

		public int Width => size.x;
		public int Height => size.y;

		public Grid()
		{
		}

		public Grid(Vector2Int size)
		{
			if (size.x < 1 || size.y < 1)
			{
				Debug.LogError(string.Format("Invalid grid storage size {0}, {1}.", size.x, size.y));
				return;
			}

			this.size = size;

			rows = new List<Row>();

			for (int i = 0; i < size.y; i++)
			{
				Row row = new Row(size.x);
				rows.Add(row);
			}
		}

		Row GetRow(int index) { return index < rows.Count ? rows[index] : null; }

		public void Resize(Vector2Int newSize)
		{
			var newWidth = newSize.x;
			var newHeight = newSize.y;

			if (newWidth < 1 || newHeight < 1)
			{
				Debug.LogError(string.Format("Invalid grid storage size {0}, {1}.", size.x, size.y));
				return;
			}

			// Same size.
			if (newWidth == Width && newHeight == Height) { return; }

			// Change width of existing rows first.
			int y = 0;
			foreach (Row row in rows)
			{
				// Don't bother resizing rows that are going to be deleted if the
				// grid is shrinking.
				if (y >= newHeight)
				{
					break;
				}

				int sizeX = Width;

				// Add new columns.
				while (row.Count < newWidth)
				{
					row.Add(new T());
					sizeX++;
				}

				// Remove deleted columns.
				if (row.Count > newWidth)
				{
					var diff = row.Count - newWidth;
					row.RemoveRange(row.Count - diff, diff);
				}

				y++;
			}

			// Add new rows.
			while (rows.Count < newHeight) {
				var row = new Row(newWidth);
				rows.Add(row);
			}
			if (rows.Count > newHeight)
			{
				var diff = rows.Count - newHeight;
				rows.RemoveRange(rows.Count - diff, diff);
			}

			size.x = newWidth;
			size.y = newHeight;
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

	class GridBool : Grid<bool>
	{

	}

	class GridInt : Grid<int>
	{

	}
}
