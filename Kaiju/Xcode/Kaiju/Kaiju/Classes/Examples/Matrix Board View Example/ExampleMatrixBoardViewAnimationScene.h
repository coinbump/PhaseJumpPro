#pragma once

#include "SDKIncludes.h"

using namespace PJ;

namespace Example {
    /**
     Builds a matrix board and animates movements while moving pieces across the board
     */
    namespace MatrixBoardViewAnimation {
        class Scene : public PJ::Scene {
            using Base = PJ::Scene;
            using This = Scene;

            Vector2 worldSize{ 800, 600 };
            Vec2I matrixSize{ 40, 30 };
            MatrixBoardView* boardView{};

            /// The number of pieces to add to the matrix board
            int count = 10;

            /// Set to true to show random movement in map directions
            /// Otherwise, movement is random across the matrix
            bool moveInDirections = false;

            // MARK: SomeWorldComponent

            void LoadInto(WorldNode& root) override;
        };
    } // namespace MatrixBoardViewAnimation
} // namespace Example
