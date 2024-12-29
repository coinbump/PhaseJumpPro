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

    template <class Enum>
    VectorList<Enum> AllCases() {
        return {};
    }

    template <>
    VectorList<MapDirection> AllCases<MapDirection>();

    /// @return Returns the opposite direction
    MapDirection Opposite(MapDirection direction);

    /// @return Returns offset in matrix space (top-left is 0, 0)
    Vector2Int MapOffset(MapDirection state);

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
        using PieceSet = UnorderedSet<SP<MatrixPiece>>;

    protected:
        MatrixType matrix;
        PieceSet pieces;

    public:
        MatrixBoard(Vector2Int size);

        MatrixType& Matrix();

        Vector2Int Size() const {
            return matrix.Size();
        }

        PieceSet const& Pieces() const {
            return pieces;
        }

        /// @return Returns true if the cell has a piece covering it
        bool IsCellOccupied(Vector2Int origin);

        /// @return Returns the cells this piece would cover in the board if it was placed at the
        /// origin
        VectorList<Vector2Int> PieceLocationsAt(Vector2Int origin, MatrixPiece const& piece);

        /// Place the piece on the board
        /// @return Returns true if placing the piece was successful
        bool Put(SP<MatrixPiece> piece, Vector2Int origin);

        /// @return Returns the piece at the specified location
        SP<MatrixPiece> GetPiece(Vector2Int location);

        /// @return Returns the piece in the map direction from the original location
        SP<MatrixPiece> PieceInDirection(Vector2Int location, MapDirection direction);

        void RemovePieceAt(Vector2Int location);
        void Remove(SP<MatrixPiece> piece);

        /// @return Returns true if the piece would be blocked if we tried to place it at this
        /// origin
        bool IsPieceBlockedAt(Vector2Int origin, MatrixPiece const& piece);

        /// @return Returns true if the piece would be blocked, ignoring a set of pieces in the
        /// check Useful when we are moving pieces in the board
        bool IsPieceBlockedAtExclude(
            Vector2Int origin, MatrixPiece const& piece, UnorderedSet<SP<MatrixPiece>> excludePieces
        );

        void SetCell(Vector2Int loc, Cell value) {
            matrix.SetCell(loc, value);
        }

        /// @return Returns the cell if location is valid. If not, throws an exception
        Cell& CellAt(Vector2Int loc) {
            return matrix.CellAt(loc);
        }

        /// @return Returns the cell if location is valid. If not, throws an exception
        Cell const& CellAt(Vector2Int loc) const {
            return matrix.CellAt(loc);
        }

        bool IsValid() const {
            return matrix.IsValid();
        }
    };
} // namespace PJ
