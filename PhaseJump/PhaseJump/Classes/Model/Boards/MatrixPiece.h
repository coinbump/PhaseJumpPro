#pragma once

#include "Dev.h"
#include "Matrix.h"
#include "Vector2.h"
#include "Void.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/8/24
 */
namespace PJ {
    class MatrixBoard;

    /// Lightweight matrix piece placed in a matrix board
    /// Example: tetromino inside a grid
    class MatrixPiece {
    public:
        using This = MatrixPiece;

        using Cell = uint8_t;

        friend class MatrixBoard;

        // Avoid Matrix<bool> because of C++ specialization
        using MatrixType = PJ::Matrix<Cell>;

    protected:
        MatrixType matrix;

        /// Location of this piece on the board
        Vector2Int origin;

    public:
        /// Board that owns this piece
        MatrixBoard* board{};

        /// (Optional) Object that owns this piece (besides the board)
        void* owner{};

        MatrixPiece(Vector2Int size) :
            matrix(size) {}

        /// @return Returns a rectangular piece
        static This Rect(Vector2Int size) {
            This result(size);
            result.Fill();
            return result;
        }

        Vector2Int Origin() const {
            return origin;
        }

        Vector2Int Size() const {
            return matrix.Size();
        }

        int Width() const {
            return matrix.Size().x;
        }

        int Height() const {
            return matrix.Size().y;
        }

        MatrixType const& Matrix() const {
            return matrix;
        }

        void Clear() {
            matrix.SetAllCells(false);
        }

        void Fill() {
            matrix.SetAllCells(true);
        }

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

        bool BuildFromShape(String pieceShape) {
            return BuildFromShape(ComponentsSeparatedBy(pieceShape, '&'));
        }

        /// Builds the piece from a list of strings that define its shape
        /// Example:
        /// *
        /// **
        /// Defines a small L shape
        bool BuildFromShape(VectorList<String> const& pieceShape) {
            int height = (int)pieceShape.size();
            int width = 0;

            for (auto& pieceString : pieceShape) {
                width = std::max(width, (int)pieceString.size());
            }

            if (height <= 0 || width <= 0) {
                PJ::Log("Error. Invalid piece shape");
                return false;
            }

            matrix.ResizeFast(Vector2Int(width, height));
            Clear();

            auto y = 0;
            for (auto& pieceString : pieceShape) {
                for (int x = 0; x < width && x < pieceString.size(); x++) {
                    auto character = pieceString[x];
                    /// Space or `_` can be used to designate empty cells
                    if (character != ' ' && character != '_') {
                        matrix.SetCell(Vector2Int(x, y), true);
                    }
                }

                y++;
            }

            return true;
        }
    };
} // namespace PJ
