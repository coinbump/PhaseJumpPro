using System;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests (via MatrixBoard)
 * CODE REVIEW: 4/23/22
 */
namespace PJ
{
    /// <summary>
    /// A matrix board piece is a grid of filled in cells
    /// Example:
    ///  *
    /// *** is a 3x2 piece with 4 filled cells
    /// </summary>
    public class MatrixPiece : Matrix<bool>
    {
        public Vector2Int origin;
        public WeakReference<MatrixBoard> board;
        public WeakReference<object> owner;

        public MatrixPiece(Vector2Int size) : base(size)
        {
        }

        public void Clear()
        {
            for (int x = 0; x < Width; x++)
            {
                for (int y = 0; y < Height; y++)
                {
                    SetCell(new Vector2Int(x, y), false);
                }
            }
        }

        public void Fill()
        {
            for (int x = 0; x < Width; x++)
            {
                for (int y = 0; y < Height; y++)
                {
                    SetCell(new Vector2Int(x, y), true);
                }
            }
        }

        /// <summary>
        /// Builds the piece from a list of strings that define its shape
        /// Example:
        /// *
        /// **
        /// Defines a small L shape
        /// </summary>
        /// <param name="pieceShape"></param>
        public bool BuildFromShape(List<string> pieceShape)
        {
            var height = pieceShape.Count;
            var width = 0;

            foreach (string pieceString in pieceShape)
            {
                width = Math.Max(width, pieceString.Length);
            }

            if (height <= 0 || width <= 0)
            {
                Debug.Log("Error. Invalid piece shape");
                return false;
            }

            Resize(new Vector2Int(width, height));
            Clear();

            var y = 0;
            foreach (string pieceString in pieceShape)
            {
                for (int x = 0; x < width && x < pieceString.Length; x++)
                {
                    var character = pieceString[x];
                    if (character != ' ')
                    {
                        SetCell(new Vector2Int(x, y), true);
                    }
                }

                y++;
            }

            return true;
        }
    }
}
