#include "ExampleMinesweeperScene.h"

using namespace std;
using namespace PJ;
using namespace Example;

void Minesweeper::Scene::RevealBombs() {
    GUARD(boardView)

    state = StateType::GameOver;

    boardView->ForEachPiece([&](auto& piece) {
        GUARD(piece.typeTags.contains("bomb"));
        piece.typeTags.insert("reveal");
    });
}

void Minesweeper::Scene::ClearTilesAt(Vec2I loc) {
    try {
        auto& node = boardView->NodeAt(loc);
        auto pieceHandler = node.TypeComponent<MatrixPieceHandler>();
        GUARD(!pieceHandler->piece->typeTags.contains("clear"));

        pieceHandler->piece->typeTags.insert("clear");
    } catch (...) {
        return;
    }

    if (BombSurroundCount(loc, true) == 0) {
        ClearTilesAt(Vector2Int(loc.x - 1, loc.y));
        ClearTilesAt(Vector2Int(loc.x + 1, loc.y));
        ClearTilesAt(Vector2Int(loc.x, loc.y - 1));
        ClearTilesAt(Vector2Int(loc.x, loc.y + 1));
    }
}

void Minesweeper::Scene::PopulateBoard() {
    GUARD(owner)

    LoadInto(*owner);
}

void Minesweeper::Scene::Restart() {
    GUARD(owner)

    owner->DestroyAllChildren();
    PopulateBoard();
    state = StateType::Running;
}

int Minesweeper::Scene::BombSurroundCount(Vector2Int loc, bool skipDiagonals) {
    auto result = 0;

    for (int y = loc.y - 1; y <= loc.y + 1; y++) {
        for (int x = loc.x - 1; x <= loc.x + 1; x++) {
            GUARD_CONTINUE(Vec2I(x, y) != loc)

            if (skipDiagonals) {
                if (x != loc.x && y != loc.y) {
                    continue;
                }
            }

            Vector2Int _loc(x, y);

            auto piece = boardView->board.GetPiece(_loc);
            auto isBomb = piece && piece->typeTags.contains("bomb");

            if (isBomb) {
                result++;
            }
        }
    }

    return result;
}

void Minesweeper::Scene::LoadInto(WorldNode& root) {
    QB(root)
        .Named("Example: Minesweeper")
        .OrthoStandard(Color32(30, 30, 30))
        .And("Minesweeper")
        .ScaleWithWindow(worldSize, 0.8f)
        .With<MatrixBoardView>(worldSize, matrixSize)
        .ModifyLatest<MatrixBoardView>([this](auto& _boardView) {
            this->boardView = &_boardView;

            QB(*_boardView.owner)
                .RectCollider(_boardView.WorldSize())
                .template With<ImRenderer>(ImRenderer::Config{ .worldSize = worldSize })
                .template ModifyLatest<ImRenderer>([&](auto& renderer) {
                    /// Optimize: draw opaque tiles behind text
                    renderer.SetAreImagesOpaque(true);

                    renderer.AddSignalHandler(
                        { .id = SignalId::RenderPrepare,
                          .func =
                              [this](auto& renderer, auto& signal) {
                                  ImRenderer& im = *(static_cast<ImRenderer*>(&renderer));
                                  imRenderer = &im;

                                  boardView->ForEachPiece([&](auto& piece) {
                                      Vec2I loc = piece.Origin();
                                      Vector2 origin{ (float)loc.x * tileSize.x * vecRight,
                                                      im.WorldSize().y + tileSize.y * vecDown +
                                                          (float)loc.y * tileSize.y * vecDown };

                                      if (piece.typeTags.contains("bomb") &&
                                          piece.typeTags.contains("reveal")) {
                                          im.Image("example-minesweeper-tile-bomb", origin);
                                      } else if (piece.typeTags.contains("clear")) {
                                          im.Image("example-minesweeper-tile-fill", origin);

                                          auto bombSurroundCount = BombSurroundCount(loc, false);
                                          if (bombSurroundCount > 0) {
                                              ModelColor hsv;
                                              hsv.value = HSVColor(
                                                  (float)bombSurroundCount / 9.0f, 1, 1, 1
                                              );
                                              ModelColor rgb = hsv.ToRGB();

                                              Color color = rgb;

                                              im.Text(
                                                  MakeString(bombSurroundCount),
                                                  Rect{ .origin = origin, .size = tileSize }, 32,
                                                  color
                                              );
                                          }
                                      } else if (piece.typeTags.contains("flag")) {
                                          im.Image("example-minesweeper-tile-flag", origin);
                                      } else {
                                          im.Image("example-minesweeper-tile-empty", origin);
                                      }
                                  });

                                  // Optimize: our scene is static, don't re-render until
                                  // something changes
                                  im.Freeze(true);
                              } }
                    );
                });

            _boardView.template AddSignalHandler<PointerDownUIEvent>(
                { .id = SignalId::PointerDown,
                  .func =
                      [this](auto& component, auto& event) {
                          auto screenPos = event.screenPos;
                          auto localPos = ScreenToLocal(*boardView, screenPos);
                          auto loc = boardView->LocalPositionToLocation(localPos);
                          GUARD(loc)

                          auto piece = boardView->board.GetPiece(*loc);
                          GUARD(piece)

                          if (PointerInputButton::Left == event.button) {
                              switch (state) {
                              case StateType::Running:
                                  if (piece->typeTags.contains("bomb")) {
                                      RevealBombs();
                                  } else {
                                      ClearTilesAt(*loc);
                                  }
                                  break;
                              case StateType::GameOver:
                                  Restart();
                              }
                          } else {
                              switch (state) {
                              case StateType::Running:
                                  {
                                      if (!piece->typeTags.contains("clear")) {
                                          bool isFlag = piece->typeTags.contains("flag");
                                          if (isFlag) {
                                              piece->typeTags.erase("flag");
                                          } else {
                                              piece->typeTags.insert("flag");
                                          }
                                      }
                                      break;
                                  }
                              case StateType::GameOver:
                                  break;
                              }
                          }

                          if (imRenderer) {
                              // Scene has changed, so allow next render
                              imRenderer->Freeze(false);
                          }
                      } }
            );
        })
        .Repeat(1, [=, this](QuickBuild& qb) {
            GUARD(boardView)

            StandardRandom random;

            VectorList<Vec2I> emptyCells;

            for (int y = 0; y < boardView->BoardSize().y; y++) {
                for (int x = 0; x < boardView->BoardSize().x; x++) {
                    Vec2I loc(x, y);

                    auto bombFactor = 0.15f;
                    bool isBomb = random.Value() < bombFactor;

                    qb.And("Matrix piece")
                        .With<MatrixPieceHandler>(MatrixPieceHandler::Config{ .origin = loc,
                                                                              .shapeString = "*" })
                        .ModifyLatest<MatrixPieceHandler>([&](auto& pieceHandler) {
                            boardView->Put(pieceHandler, pieceHandler.startOrigin);

                            if (isBomb) {
                                pieceHandler.piece->typeTags.insert("bomb");
                            } else {
                                emptyCells.push_back(loc);
                            }
                        })
                        .Pop();
                }
            }

            // Give the player a hint by clearing something for them
            std::sort(emptyCells.begin(), emptyCells.end(), [this](auto lhs, auto rhs) {
                return BombSurroundCount(lhs, false) < BombSurroundCount(rhs, false);
            });
            ClearTilesAt(emptyCells[0]);
        });
}
