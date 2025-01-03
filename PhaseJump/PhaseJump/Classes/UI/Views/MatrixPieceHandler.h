#pragma once

#include "MatrixBoard.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/17/24
 */
namespace PJ {
    /// Attaches matrix piece info to a world node
    /// Keeps track of the animator for move animations for that piece
    class MatrixPieceHandler : public WorldComponent<> {
    protected:
        SP<Updatable> animator;

    public:
        /// Initial position for piece in matrix
        Vector2Int startOrigin;

        /// Piece shape
        SP<MatrixPiece> piece{};

        struct Config {
            Vector2Int origin;
            Vector2Int size;

            /// Shape is built from a list of rows
            /// Example: {`*`, `** *`}
            VectorList<String> shape;

            /// Shape is built from a single string with `&` delimiters between rows
            /// Example: `**& *`
            String shapeString;

            /// (Optional). Existing piece object. Use to inject custom types
            SP<MatrixPiece> piece;
        };

        MatrixPieceHandler(Config config) :
            startOrigin(config.origin),
            piece(config.piece) {
            if (!piece) {
                piece = MAKE<MatrixPiece>(config.size);
            }

            if (piece) {
                piece->owner = this;

                if (!IsEmpty(config.shape)) {
                    piece->BuildFromShape(config.shape);
                } else if (!IsEmpty(config.shapeString)) {
                    piece->BuildFromShape(config.shapeString);
                }
            }
        }

        /// Stores the animator and keeps it updated for this piece
        /// Replaces the existing animator if it is currently animating
        void SetAnimator(SP<Updatable> value);

        bool IsAnimating() const {
            return animator && !animator->IsFinished();
        }

        Vec2I Size() const {
            return piece ? piece->Size() : Vec2I{ 0, 0 };
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "MatrixPieceHandler";
        }
    };
} // namespace PJ
