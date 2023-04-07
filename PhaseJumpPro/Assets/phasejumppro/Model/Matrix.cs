using UnityEngine;
using System.Collections;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/16/22
 */
namespace PJ
{
    public class SomeMatrix
    {
        public enum RotateDirection
        {
            Left, Right
        }
    }

    /// <summary>
    /// Stores objects in an X,Y matrix structure (Y rows of X cells)
    /// </summary>
    /// <typeparam name="T">Type of thing to store</typeparam>
    public class Matrix<T> : SomeMatrix where T : new()
    {
        public class Row : List<T>
        {
            public Row(int capacity) : base(capacity)
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
        public Vector2Int Size => size;

        public Matrix(Vector2Int matrixSize)
        {
            var size = new Vector2Int(System.Math.Max(1, matrixSize.x), System.Math.Max(1, matrixSize.y));
            this.size = size;

            rows = new List<Row>();

            for (int i = 0; i < size.y; i++)
            {
                Row row = new Row(size.x);
                rows.Add(row);
            }
        }

        public Row RowAt(int index) { return index < rows.Count ? rows[index] : null; }

        public void Rotate(RotateDirection direction)
        {
            List<T> cells = new List<T>();
            List<Vector2Int> locations = new List<Vector2Int>();

            for (int x = 0; x < size.x; x++)
            {
                for (int y = 0; y < size.y; y++)
                {
                    var location = new Vector2Int(x, y);
                    var myCell = CellAt(location);
                    cells.Add(myCell);

                    locations.Add(location);
                }
            }

            var oldWidth = Width;
            var oldHeight = Height;

            var newSize = new Vector2Int(size.y, size.x);
            Resize(newSize);

            for (int i = 0; i < cells.Count; i++)
            {
                var oldLocation = locations[i];
                var cell = cells[i];

                Vector2Int newLocation = new Vector2Int(0, 0);

                switch (direction)
                {
                    case RotateDirection.Left:
                        newLocation = new Vector2Int(oldLocation.y, Height - oldLocation.x - 1);
                        break;
                    case RotateDirection.Right:
                        newLocation = new Vector2Int(Width - 1 - oldLocation.y, oldLocation.x);
                        break;
                }

                SetCell(newLocation, cell);
            }
        }

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

                // Add new columns
                while (row.Count < newWidth)
                {
                    row.Add(new T());
                    sizeX++;
                }

                // Remove deleted columns
                if (row.Count > newWidth)
                {
                    var diff = row.Count - newWidth;
                    row.RemoveRange(row.Count - diff, diff);
                }

                y++;
            }

            // Add new rows
            while (rows.Count < newHeight)
            {
                var row = new Row(newWidth);
                rows.Add(row);
            }

            // Remove deleted rows
            if (rows.Count > newHeight)
            {
                var diff = rows.Count - newHeight;
                rows.RemoveRange(rows.Count - diff, diff);
            }

            size.x = newWidth;
            size.y = newHeight;
        }

        public bool IsValidLocation(Vector2Int loc)
        {
            return (loc.x >= 0 && loc.x < Width &&
                    loc.y >= 0 && loc.y < Height);
        }

        public virtual T CellAt(Vector2Int loc)
        {
            if (IsValidLocation(loc))
            {
                Row row = rows[loc.y];
                return row[loc.x];
            }

            return new T();
        }

        public virtual void SetCell(Vector2Int loc, T content)
        {
            if (IsValidLocation(loc))
            {
                Row row = RowAt(loc.y);
                if (null != row)
                {
                    row[loc.x] = content;
                }
            }
        }
    }

    /// <summary>
    /// Matrix of bools
    /// </summary>
    public class MatrixBool : Matrix<bool>
    {
        public MatrixBool(Vector2Int matrixSize) : base(matrixSize)
        {
        }
    }

    /// <summary>
    /// Matrix of ints
    /// </summary>
    public class MatrixInt : Matrix<int>
    {
        public MatrixInt(Vector2Int matrixSize) : base(matrixSize)
        {
        }
    }
}
