#pragma once

#include "SDKIncludes.h"

namespace Example {
    /**
     Builds a mine sweeper game example
     */
    namespace Minesweeper {
        class Scene : public PJ::Scene {
        public:
            using Base = PJ::Scene;
            using This = Scene;

            Vector2 worldSize{ 1200, 1200 };
            Vec2I matrixSize{ 24, 24 };
            Vector2 tileSize{ 50, 50 };

            MatrixBoardView* boardView{};

            ImRenderer* imRenderer{};

            enum class StateType {
                Running,

                GameOver
            };

            StateType state{};

            // MARK: SomeWorldComponent

            void LoadInto(WorldNode& root) override;

        protected:
            /// Reveals all bombs on the board
            void RevealBombs();

            /// Clears tiles starting from the specified location
            void ClearTilesAt(Vec2I loc);

            /// Restarts the game
            void Restart();

            /// Builds the game board
            void PopulateBoard();

            /// @return Returns the # of bombs surrounding the location
            int BombSurroundCount(Vector2Int loc, bool skipDiagonals);
        };
    } // namespace Minesweeper
} // namespace Example
