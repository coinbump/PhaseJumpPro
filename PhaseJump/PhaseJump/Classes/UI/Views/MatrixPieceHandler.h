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
    /// Keep strack of the animator for move animations for that piece
    class MatrixPieceHandler : public WorldComponent<> {
    protected:
        SP<Updatable> animator;

    public:
        /// Initial position for piece in matrix
        Vector2Int startOrigin;

        /// Piece shape
        SP<MatrixPiece> piece{};

        MatrixPieceHandler(Vector2Int startOrigin, Vector2Int size) :
            startOrigin(startOrigin) {
            piece = MAKE<MatrixPiece>(size);
            piece->owner = this;
        }

        MatrixPieceHandler(Vector2Int startOrigin, VectorList<String> shape) :
            startOrigin(startOrigin) {
            piece = MAKE<MatrixPiece>(Vector2Int(0, 0));
            piece->owner = this;

            piece->BuildFromShape(shape);
        }

        MatrixPieceHandler(Vector2Int startOrigin, String shape) :
            startOrigin(startOrigin) {
            piece = MAKE<MatrixPiece>(Vector2Int(0, 0));
            piece->owner = this;

            piece->BuildFromShape(shape);
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

        void OnUpdate(TimeSlice time) override;
    };
} // namespace PJ
