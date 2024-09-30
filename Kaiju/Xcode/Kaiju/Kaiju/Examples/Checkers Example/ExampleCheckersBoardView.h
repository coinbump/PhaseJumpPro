#pragma once

#if DEVELOPMENT
#include <PhaseJump-Dev/PhaseJump-Dev.h>
#else
#include <PhaseJump/PhaseJump.h>
#endif

#include "ExampleCheckersMatrixRenderer.h"

namespace Example {
    namespace Checkers {
        class PiecesView;

        /// Contains square matrix pieces with drop targets so have a drag destination for the
        /// checkers pieces Each piece represents a playable square on the board
        class BoardView : public MatrixBoardView {
        public:
            using Base = MatrixBoardView;

            PiecesView* piecesView{};

            BoardView(Vector2 worldSize, PiecesView* piecesView) :
                Base(worldSize, { 8, 8 }),
                piecesView(piecesView) {}

            // MARK: SomeWorldComponent

            void Awake() override;

        protected:
            void Build();
        };
    } // namespace Checkers
} // namespace Example
