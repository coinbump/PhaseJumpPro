#pragma once

#include "MatrixPiece.h"
#include "UnorderedSet.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/8/24
 */
namespace PJ {
    enum class MapDirection {
        Northwest,
        North,
        Northeast,
        East,
        South,
        Southeast,
        Southwest,
        West
    };

    /// Returns the opposite direction
    MapDirection Opposite(MapDirection direction) {
        switch (direction) {
        case MapDirection::East:
            return MapDirection::West;
        case MapDirection::West:
            return MapDirection::East;
        case MapDirection::Northwest:
            return MapDirection::Southeast;
        case MapDirection::North:
            return MapDirection::South;
        case MapDirection::Northeast:
            return MapDirection::Southwest;
        case MapDirection::Southeast:
            return MapDirection::Northwest;
        case MapDirection::South:
            return MapDirection::North;
        case MapDirection::Southwest:
            return MapDirection::Northeast;
        }

        return MapDirection::North;
    }

    /// Returns offset in matrix space (top-left is 0, 0)
    Vector2Int MapOffset(MapDirection state) {
        switch (state) {
        case MapDirection::Northwest:
            return Vector2Int(-1, -1);
        case MapDirection::North:
            return Vector2Int(0, -1);
        case MapDirection::Northeast:
            return Vector2Int(1, -1);
        case MapDirection::East:
            return Vector2Int(1, 0);
        case MapDirection::Southeast:
            return Vector2Int(1, 1);
        case MapDirection::South:
            return Vector2Int(0, 1);
        case MapDirection::Southwest:
            return Vector2Int(-1, 1);
        case MapDirection::West:
            return Vector2Int(-1, 0);
        }

        return Vector2Int(0, 0);
    }

    /// Model for placing multi-cell pieces inside a matrix
    /// Example: tetrominoes-type game
    /// If all your pieces are single cells, use Matrix directly
    class MatrixBoard {
    public:
        /// Stores a link to the piece and which part of the piece this cell is referencing
        struct Cell {
            SP<MatrixPiece> piece;

            /// Interior location inside the piece that corresponds to this cell
            Vector2Int pieceLoc;
        };

        using MatrixType = PJ::Matrix<Cell>;

    protected:
        MatrixType matrix;
        UnorderedSet<SP<MatrixPiece>> pieces;

    public:
        MatrixBoard(Vector2Int size);

        MatrixType& Matrix();

        /// Returns true if the cell has a piece covering it
        bool IsCellOccupied(Vector2Int origin);

        /// Returns the cells this piece would cover in the board if it was placed at the
        /// origin
        VectorList<Vector2Int> PieceLocationsAt(Vector2Int origin, MatrixPiece const& piece);

        /// Place the piece on the board
        /// Returns true if placing the piece was successful
        bool PutPiece(SP<MatrixPiece> piece, Vector2Int origin);

        /// Returns the piece at the specified location
        SP<MatrixPiece> PieceAt(Vector2Int location);

        /// Returns the piece in the map direction from the original location
        SP<MatrixPiece> PieceInDirection(Vector2Int location, MapDirection direction);

        void RemovePieceAt(Vector2Int location);
        void Remove(SP<MatrixPiece> piece);

        /// Returns true if the piece would be blocked if we tried to place it at this origin
        bool IsPieceBlockedAt(Vector2Int origin, MatrixPiece const& piece);

        /// Returns true if the piece would be blocked, ignoring a set of pieces in the check
        /// Useful when we are moving pieces in the board
        bool IsPieceBlockedAtExclude(
            Vector2Int origin, MatrixPiece const& piece, UnorderedSet<SP<MatrixPiece>> excludePieces
        );

        void SetCell(Vector2Int loc, Cell value) {
            matrix.SetCell(loc, value);
        }

        /// Returns the cell if location is valid. If not, throws an exception
        Cell& CellAt(Vector2Int loc) {
            return matrix.CellAt(loc);
        }

        /// Returns the cell if location is valid. If not, throws an exception
        Cell const& CellAt(Vector2Int loc) const {
            return matrix.CellAt(loc);
        }
    };
} // namespace PJ
