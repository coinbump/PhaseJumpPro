using System;
using Godot;

namespace PJ
{
    /// <summary>
    /// Utility functions for axial movement in matrix space
    /// </summary>
    public class MatrixAxial
    {
        /*
           AXIAL:
           Axial directions can be used to navigate a grid by moving in a specific
           direction (North, Northeast, etc.)
       */
        // 	ORDER: top-left, running clockwise.
        public static readonly Vector2I[] NeighborAxialLocs = {
                 new Vector2I(-1, -1),
                 new Vector2I(0, -1),
                 new Vector2I(1, -1),
                 new Vector2I(1, 0),
                 new Vector2I(1, 1),
                 new Vector2I(0, 1),
                 new Vector2I(-1, 1),
                 new Vector2I(-1, 0)
             };

        /// <summary>
        /// Number of axial directions from one cell to another (North, Northeast, etc.)
        /// </summary>
        public virtual int AxialDirectionsCount
        {
            get => 8;
        }

        public bool DoesAxialIndexMatchType(int index, AxialType type)
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

        public Vector2I AxialFor(int index)
        {
            Vector2I result = new Vector2I(0, 0);
            return index < 0 || index > (NeighborAxialLocs.Length - 1) ? result : NeighborAxialLocs[index];
        }

        public Optional<int> AxialIndexFor(Vector2I axial)
        {
            for (int i = 0; i < AxialDirectionsCount; i++)
            {
                Vector2I axialOffset = NeighborAxialLocs[i];
                if (axialOffset == axial)
                {
                    return new Optional<int>(i);
                }
            }

            return null;  // Invalid.
        }

        public int NextAxialIndex(int axialIndex, AxialDirection dir)
        {
            switch (dir)
            {
                case AxialDirection.Right:
                    axialIndex++;
                    break;
                default:
                    axialIndex--;
                    if (axialIndex < 0)
                    {
                        axialIndex = AxialDirectionsCount - 1;
                    }
                    break;
            }

            axialIndex %= AxialDirectionsCount;

            return axialIndex;
        }

        public Vector2I NextAxial(int axialIndex, AxialDirection dir)
        {
            int nextIndex = NextAxialIndex(axialIndex, dir);
            return NeighborAxialLocs[nextIndex];
        }

        public Vector2I OffsetLoc(Vector2I loc, Vector2I offset)
        {
            Vector2I result = loc;
            result.X += offset.X;
            result.Y += offset.Y;
            return result;
        }
    }
}
