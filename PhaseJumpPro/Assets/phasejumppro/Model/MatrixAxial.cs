using System;
using UnityEngine;

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
        public static readonly Vector2Int[] NeighborAxialLocs = {
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

        public Vector2Int AxialFor(int index)
        {
            Vector2Int result = new Vector2Int(0, 0);
            return index < 0 || index > (NeighborAxialLocs.Length - 1) ? result : NeighborAxialLocs[index];
        }

        public Optional<int> AxialIndexFor(Vector2Int axial)
        {
            for (int i = 0; i < AxialDirectionsCount; i++)
            {
                Vector2Int axialOffset = NeighborAxialLocs[i];
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

        public Vector2Int NextAxial(int axialIndex, AxialDirection dir)
        {
            int nextIndex = NextAxialIndex(axialIndex, dir);
            return NeighborAxialLocs[nextIndex];
        }

        public Vector2Int OffsetLoc(Vector2Int loc, Vector2Int offset)
        {
            Vector2Int result = loc;
            result.x += offset.x;
            result.y += offset.y;
            return result;
        }
    }
}
