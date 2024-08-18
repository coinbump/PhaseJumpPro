#pragma once

#include "Matrix.h"
#include "Vector2.h"

// CODE REVIEW: ?/23
namespace PJ {
    // TODO: needs unit tests
    class MatrixBoard;

    /// A matrix board piece is a grid of filled in cells
    /// Example:
    ///  *
    /// *** is a 3x2 piece with 4 filled cells
    class MatrixPiece {
        Matrix<bool> matrix;
        Vector2Int origin;
        WP<MatrixBoard> board;

        MatrixPiece(Vector2Int size) :
            matrix(size) {}

        void Clear() {
            for (int y = 0; y < matrix.Height(); y++) {
                *matrix.TupleAt(y) = VectorList<bool>(matrix.Width(), false);
            }
        }

        void Fill() {
            for (int y = 0; y < matrix.Height(); y++) {
                *matrix.TupleAt(y) = VectorList<bool>(matrix.Width(), true);
            }
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
                std::cout << "Error. Invalid piece shape";
                return false;
            }

            matrix.Resize(Vector2Int((int)width, (int)height));
            Clear();

            auto y = 0;
            for (auto& pieceString : pieceShape) {
                for (int x = 0; x < width && x < pieceString.size(); x++) {
                    auto character = pieceString[x];
                    if (character != ' ') {
                        matrix.SetCell(Vector2Int(x, y), true);
                    }
                }

                y++;
            }

            return true;
        }
    };
} // namespace PJ
