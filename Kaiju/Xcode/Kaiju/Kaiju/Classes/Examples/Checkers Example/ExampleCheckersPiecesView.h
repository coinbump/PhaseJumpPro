#pragma once

#include "ExampleCheckersMatrixRenderer.h"
#include "SDKIncludes.h"

namespace Example {
    namespace Checkers {
        // Matrix view for draggable checkers pieces
        class PiecesView : public MatrixBoardView {
        public:
            using Base = MatrixBoardView;

            PiecesView(Vector2 worldSize) :
                Base(worldSize, { 8, 8 }) {}

            bool IsDarkSquareAt(Vec2I location);
            void TryMove(MatrixPieceHandler& pieceHandler, Vec2I location);
            bool CanMove(MatrixPieceHandler& pieceHandler, Vec2I location);

            // MARK: SomeWorldComponent

            void Awake() override;

        protected:
            void Build();
        };
    } // namespace Checkers
} // namespace Example
