#include "ExampleMineSweeperScene.h"

// #if FALSE
namespace Example {
    namespace MineSweeper {
        class Tile : public MatrixPieceHandler {
        public:
            enum class TileType { Tile, Bomb, Clear, ClearFlag, BombFlag };

            TileType type = TileType::Tile;

            bool IsBomb() const {
                switch (type) {
                case TileType::Bomb:
                case TileType::BombFlag:
                    return true;
                default:
                    return false;
                }
            }
        };

        /// Uses Matrix Board View to play MineSweeper
        /// How to Play: Left click to guess at an empty space, right click to flag a potential bomb
        /// location Failure: when you tap a bomb -> Game Over.
        class ExampleMineSweeperView : public MatrixBoardView {
        public:
            using Base = MatrixBoardView;

            enum class StateType { Running, GameOver };

            StateMachine<StateType> states;

            //     class BombCountComparer : IComparer<Vector2Int>
            //     {
            //         EMSBoardView boardView;

            //         BombCountComparer(EMSBoardView boardView)
            //         {
            //             this.boardView = boardView;
            //         }

            //         int Compare(Vector2Int lhs, Vector2Int rhs)
            //         {
            //             return boardView.BombSurroundCount(lhs, false) <
            //             boardView.BombSurroundCount(rhs, false) ? -1 : 1;
            //         }
            //     }

            void Start() override {
                Base::Start();

                PopulateBoard();
            }

            void PopulateBoard() {
                Vec2i matrixSize;
                auto cellCount = matrixSize.x * matrixSize.y;
                auto bombPercentage = 0.15f;
                auto bombCount = bombPercentage * cellCount;

                VectorList<Vector2Int> emptyCells;

                for (int y = 0; y < matrixSize.y; y++) {
                    for (int x = 0; x < matrixSize.x; x++) {
                        emptyCells.push_back(Vector2Int(x, y));
                    }
                }

                for (int i = 0; i < bombCount; i++) {
                    if (emptyCells.size() == 0) {
                        break;
                    }
                    //                 auto choiceIndex = Random.Range(0, emptyCells.size());
                    //
                    //                 auto choice = emptyCells[choiceIndex];
                    //                 emptyCells.RemoveAt(choiceIndex);
                    //
                    //                 auto bombTileObject =
                    //                 sceneObjectRegistry.InstantiateGameObject("tile",
                    //                 Vector3.Zero, Quaternion.identity); auto bombTile =
                    //                 bombTileObject.GetComponent<EMSTile>(); bombTile.type =
                    //                 Tile::TileType::Bomb; Put(bombTile, choice);
                }

                for (auto& cell : emptyCells) {
                    // TODO: put tiles and populate
                    //                 tileObject.GetComponent<EMSTile>(); Put(tile, cell);
                }

                // Give the player a hint by clearing something for them
                //             emptyCells.Sort(new BombCountComparer(this));
                //             ClearTilesAt(emptyCells[0]);
            }

            void OnPointerClickEvent(PointerClickUIEvent const& event) {
                //                auto localPressPosition = _event.pressPosition;
                //                auto cellHit =
                //                LocalPositionToLocation(localPressPosition.Position);

                //                if (cellHit.value == nullptr) { return; }

                //                auto cellGameObject = NodeAt(cellHit.value);
                //                auto hitTile = cellGameObject.GetComponent<EMSTile>();

                //                switch (_event.Button)
                //                {
                //                    case PointerInputButton.Left:
                //                    {
                //                        switch (core.State)
                //                        {
                //                            case StateType::Running:
                //                                if (hitTile.type == Tile::TileType::Bomb)
                //                                {
                //                                    RevealBombs();
                //                                }
                //                                else
                //                                {
                //                                    ClearTilesAt(cellHit.value);
                //                                }
                //                                break;
                //                            case StateType::GameOver:
                //                                Restart();
                //                                break;
                //                        }
                //                        break;
                //                    }
                //                    case PointerInputButton.Right:
                //                    {
                //                        switch (hitTile.type)
                //                        {
                //                            case Tile::TileType::Tile:
                //                            {
                //                                // TODO: change to flag
                //                            }
                //                            case Tile::TileType::Bomb:
                //                            {
                //                                // TODO: change to flag
                //                            }
                //                            case Tile::TileType::ClearFlag:
                //                            {
                //                                // TODO: change to clear
                //                            }
                //                            case Tile::TileType::BombFlag:
                //                            {
                //                                // TODO: change to tile
                //                                break;
                //                            }
                //                            default:
                //                                break;
                //                        }
                //                    }
                //                        break;
                //                }
            }

            void RemoveAll() {
                // TODO:
            }

            void Restart() {
                RemoveAll();
                PopulateBoard();
                states.SetState(StateType::Running);
            }

            void RevealBombs() {
                states.SetState(StateType::GameOver);

                // TODO:
                Vec2i matrixSize;

                for (int y = 0; y < matrixSize.y; y++) {
                    for (int x = 0; x < matrixSize.x; x++) {
                        Vector2Int cell(x, y);
                        try {
                            auto& node = NodeAt(cell);
                            //                            auto tile = node.GetComponent<EMSTile>();
                            //
                            //                            if (tile.IsBomb()) {
                            //                                // TODO: change the cell's texture
                            //                            }
                        } catch (...) {}
                    }
                }
            }

            void ClearTilesAt(Vector2Int cell) {
                ClearUnclearedTileAt(cell);
            }

            void ClearUnclearedTileAt(Vector2Int cell) {
                try {
                    auto& node = NodeAt(cell);

                    //                    auto tile = node.GetComponent<EMSTile>();
                    //                    if (tile.type == Tile::TileType::Tile)
                    {
                        // TODO: change to clear tile texture
                        //                        auto clearTileObject =
                        //                        Replace(clearTile, cell);

                        auto bombSurroundCount = BombSurroundCount(cell, false);
                        //                        auto tmp =
                        //                        clearTile.text.GetComponent<TextMeshPro>();

                        Color color = Color::red;
                        //                        Color::RGBToHSV(color, out float h, out float s,
                        //                        out float v); h = 1.0f - (Mathf.Min(4.0f,
                        //                        (float)bombSurroundCount)
                        //                                    / 4.0f);

                        //                        tmp.color = Color::HSVToRGB(h, s, v);
                        //
                        //                        switch (bombSurroundCount)
                        //                        {
                        //                            case 0:
                        //                                Destroy(clearTile.text);
                        //                                clearTile.text = nullptr;
                        //                                break;
                        //                            default:
                        //                                tmp.text = bombSurroundCount.ToString();
                        //                                break;
                        //                        }

                        if (BombSurroundCount(cell, true) == 0) {
                            ClearUnclearedTileAt(Vector2Int(cell.x - 1, cell.y));
                            ClearUnclearedTileAt(Vector2Int(cell.x + 1, cell.y));
                            ClearUnclearedTileAt(Vector2Int(cell.x, cell.y - 1));
                            ClearUnclearedTileAt(Vector2Int(cell.x, cell.y + 1));
                        }
                    }
                } catch (...) {}
            }

            int BombSurroundCount(Vector2Int cell, bool skipDiagonals) {
                auto result = 0;

                for (int y = cell.y - 1; y <= cell.y + 1; y++) {
                    for (int x = cell.x - 1; x <= cell.x + 1; x++) {
                        GUARD_CONTINUE(Vec2i(x, y) != cell)

                        if (skipDiagonals) {
                            if (x != cell.x && y != cell.y) {
                                continue;
                            }
                        }

                        //                        auto tileObject = NodeAt(Vector2Int(x, y));
                        //                        if (nullptr == tileObject) { continue; }
                        //
                        //                        auto tile = tileObject.GetComponent<EMSTile>();
                        //                        if (tile.IsBomb()())
                        //                        {
                        //                            result++;
                        //                        }
                    }
                }

                return result;
            }
        };
    } // namespace MineSweeper
} // namespace Example
