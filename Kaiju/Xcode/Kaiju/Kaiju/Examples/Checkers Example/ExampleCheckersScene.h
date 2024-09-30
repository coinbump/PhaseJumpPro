#pragma once

#if DEVELOPMENT
#include <PhaseJump-Dev/PhaseJump-Dev.h>
#else
#include <PhaseJump/PhaseJump.h>
#endif

#include "ExampleCheckersMatrixRenderer.h"

namespace Example {
    namespace Checkers {
        /**
         A subset of the game of checkers, to demonstrate layered matrix views and drag and drop
         Black and red pieces can move diagonally forward
         A piece can jump a piece of the opposing color to eliminate that piece from the board

         Multi jumps and kings are not supported
         */
        class Scene : public PJ::Scene {
            Vector2 worldSize{ 600, 600 };
            MatrixBoardView* boardView{};

            // MARK: SomeWorldComponent

            void LoadInto(WorldNode& root) override;
        };
    } // namespace Checkers
} // namespace Example
