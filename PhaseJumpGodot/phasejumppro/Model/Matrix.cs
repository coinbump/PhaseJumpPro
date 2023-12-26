using Godot;
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
    /// Stores objects in an X,Y matrix structure (Y tuples of X cells)
    /// </summary>
    /// <typeparam name="T">Type of thing to store</typeparam>
    public class Matrix<T> : SomeMatrix where T : new()
    {
        public class Tuple : List<T>
        {
            public Tuple(int capacity) : base(capacity)
            {
                // Fill the tuple (initializing with capacity doesn't do this)
                while (Count < capacity)
                {
                    Add(new T());
                }
            }
        }

        protected List<Tuple> tuples;
        protected Vector2I size;

        public int Width => size.X;
        public int Height => size.Y;
        public Vector2I Size => size;

        public Matrix(Vector2I matrixSize)
        {
            var size = new Vector2I(System.Math.Max(1, matrixSize.X), System.Math.Max(1, matrixSize.Y));
            this.size = size;

            tuples = new List<Tuple>();

            for (int i = 0; i < size.Y; i++)
            {
                Tuple tuple = new Tuple(size.X);
                tuples.Add(tuple);
            }
        }

        public Tuple TupleAt(int index) { return index < tuples.Count ? tuples[index] : null; }

        public void Rotate(RotateDirection direction)
        {
            List<T> cells = new List<T>();
            List<Vector2I> locations = new List<Vector2I>();

            for (int x = 0; x < size.X; x++)
            {
                for (int y = 0; y < size.Y; y++)
                {
                    var location = new Vector2I(x, y);
                    var myCell = CellAt(location);
                    cells.Add(myCell);

                    locations.Add(location);
                }
            }

            var oldWidth = Width;
            var oldHeight = Height;

            var newSize = new Vector2I(size.Y, size.X);
            Resize(newSize);

            for (int i = 0; i < cells.Count; i++)
            {
                var oldLocation = locations[i];
                var cell = cells[i];

                Vector2I newLocation = new Vector2I(0, 0);

                switch (direction)
                {
                    case RotateDirection.Left:
                        newLocation = new Vector2I(oldLocation.Y, Height - oldLocation.X - 1);
                        break;
                    case RotateDirection.Right:
                        newLocation = new Vector2I(Width - 1 - oldLocation.Y, oldLocation.X);
                        break;
                }

                SetCell(newLocation, cell);
            }
        }

        public void Resize(Vector2I newSize)
        {
            var newWidth = newSize.X;
            var newHeight = newSize.Y;

            if (newWidth < 1 || newHeight < 1)
            {
                Debug.LogError(string.Format("Invalid grid storage size {0}, {1}.", size.X, size.Y));
                return;
            }

            // Same size.
            if (newWidth == Width && newHeight == Height) { return; }

            // Change width of existing tuples first.
            int y = 0;
            foreach (Tuple tuple in tuples)
            {
                // Don't bother resizing tuples that are going to be deleted if the
                // grid is shrinking.
                if (y >= newHeight)
                {
                    break;
                }

                int sizeX = Width;

                // Add new columns
                while (tuple.Count < newWidth)
                {
                    tuple.Add(new T());
                    sizeX++;
                }

                // Remove deleted columns
                if (tuple.Count > newWidth)
                {
                    var diff = tuple.Count - newWidth;
                    tuple.RemoveRange(tuple.Count - diff, diff);
                }

                y++;
            }

            // Add new tuples
            while (tuples.Count < newHeight)
            {
                var tuple = new Tuple(newWidth);
                tuples.Add(tuple);
            }

            // Remove deleted tuples
            if (tuples.Count > newHeight)
            {
                var diff = tuples.Count - newHeight;
                tuples.RemoveRange(tuples.Count - diff, diff);
            }

            size.X = newWidth;
            size.Y = newHeight;
        }

        public bool IsValidLocation(Vector2I loc)
        {
            return (loc.X >= 0 && loc.X < Width &&
                    loc.Y >= 0 && loc.Y < Height);
        }

        public virtual T CellAt(Vector2I loc)
        {
            if (IsValidLocation(loc))
            {
                Tuple tuple = tuples[loc.Y];
                return tuple[loc.X];
            }

            return new T();
        }

        public virtual void SetCell(Vector2I loc, T content)
        {
            if (IsValidLocation(loc))
            {
                Tuple tuple = TupleAt(loc.Y);
                if (null != tuple)
                {
                    tuple[loc.X] = content;
                }
            }
        }
    }

    /// <summary>
    /// Matrix of bools
    /// </summary>
    public class MatrixBool : Matrix<bool>
    {
        public MatrixBool(Vector2I matrixSize) : base(matrixSize)
        {
        }
    }

    /// <summary>
    /// Matrix of ints
    /// </summary>
    public class MatrixInt : Matrix<int>
    {
        public MatrixInt(Vector2I matrixSize) : base(matrixSize)
        {
        }
    }
}
