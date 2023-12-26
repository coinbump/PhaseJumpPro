using System;
using System.Collections.Generic;
using Godot;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/23/22
 */
namespace PJ
{
    public class MatrixBoard : Matrix<MatrixBoard.Cell>
    {
        /// <summary>
        /// Has a link to the piece and which part of the piece this cell is referencing
        /// </summary>
        public class Cell
        {
            public MatrixPiece piece;

            /// <summary>
            /// Interior location inside the piece that corresponds to this cell
            /// </summary>
            public Vector2I pieceLocation;
        }

        public MatrixBoard(Vector2I size) : base(size)
        {
        }

        public bool IsCellOccupied(Vector2I origin)
        {
            if (!IsValidLocation(origin)) { return true; }

            var cell = CellAt(origin);
            if (null == cell) { return false; }

            return cell.piece != null;
        }

        public bool IsPieceBlockedAt(Vector2I origin, MatrixPiece piece)
        {
            return IsPieceBlockedAt(origin, piece, new HashSet<MatrixPiece>());
        }

        /// <summary>
        /// Returns the cells this piece would cover in the board if it was placed at the origin, or null if can't be placed there
        /// </summary>
        public List<Vector2I> PieceLocationsAt(Vector2I origin, MatrixPiece piece)
        {
            List<Vector2I> result = new();

            for (int x = 0; x < piece.Width; x++)
            {
                for (int y = 0; y < piece.Height; y++)
                {
                    var pieceLocation = new Vector2I(x, y);
                    var isOccupiedByPiece = piece.CellAt(pieceLocation);
                    if (!isOccupiedByPiece) { continue; }

                    var location = origin + pieceLocation;
                    if (!IsValidLocation(location))
                    {
                        return null;
                    }
                    result.Add(location);
                }
            }

            return result;
        }

        public bool IsPieceBlockedAt(Vector2I origin, MatrixPiece piece, HashSet<MatrixPiece> excludePieces)
        {
            for (int x = 0; x < piece.Width; x++)
            {
                for (int y = 0; y < piece.Height; y++)
                {
                    var pieceLocation = new Vector2I(x, y);
                    var isOccupiedByPiece = piece.CellAt(pieceLocation);
                    if (!isOccupiedByPiece) { continue; }

                    var location = origin + pieceLocation;

                    if (IsCellOccupied(location))
                    {
                        // If specified, ignore some pieces for check
                        if (excludePieces.Count > 0)
                        {
                            var cell = CellAt(location);
                            if (null != cell)
                            {
                                if (excludePieces.Contains(cell.piece))
                                {
                                    continue;
                                }
                            }
                        }

                        return true;
                    }
                }
            }

            return false;
        }

        public bool PutPiece(MatrixPiece piece, Vector2I origin)
        {
            if (IsPieceBlockedAt(origin, piece)) { return false; }
            if (piece.board != null) { return false; }

            piece.board = new WeakReference<MatrixBoard>(this);
            piece.origin = origin;

            for (int x = 0; x < piece.Width; x++)
            {
                for (int y = 0; y < piece.Height; y++)
                {
                    var pieceLocation = new Vector2I(x, y);
                    var isOccupiedByPiece = piece.CellAt(pieceLocation);
                    if (!isOccupiedByPiece) { continue; }

                    var cell = CellAt(origin + pieceLocation);
                    cell.piece = piece;
                    cell.pieceLocation = pieceLocation;
                }
            }

            return true;
        }

        public MatrixPiece PieceAt(Vector2I location)
        {
            var cell = CellAt(location);
            if (null == cell) { return null; }

            return cell.piece;
        }

        public MatrixPiece PieceInDirection(Vector2I location, MapDirection direction)
        {
            Vector2I offset = direction.Offset();
            var checkLocation = location + offset;
            return PieceAt(checkLocation);
        }

        public void RemovePieceAt(Vector2I location)
        {
            RemovePiece(PieceAt(location));
        }

        public void RemovePiece(MatrixPiece piece)
        {
            if (null == piece) { return; }
            if (piece.board.TryGetTarget(out MatrixBoard ownerBoard))
            {
                if (ownerBoard != this)
                {
                    GD.Print("Error. Can't remove a piece from a different board");
                    return;
                }
            }
            piece.board = null;

            var pieceLocations = PieceLocationsAt(piece.origin, piece);
            foreach (var location in pieceLocations)
            {
                var cell = CellAt(location);
                if (cell.piece != piece)
                {
                    GD.Print("Error. Cell/piece mismatch");
                    continue;
                }
                cell.piece = null;
            }
        }
    }
}
