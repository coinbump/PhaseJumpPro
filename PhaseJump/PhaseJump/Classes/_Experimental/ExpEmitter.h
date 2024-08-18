
#pragma once

#include "AtlasTexture.h"
#include "EmitModel.h"
#include "Emitter.h"
#include "List.h"
#include "PublishedValue.h"
#include "QuadMeshBuilder.h"
#include "RateLimiter.h"
#include "Runner.h"
#include "Scene.h"
#include "SomeCamera.h"
#include "SomeMeshBuilder.h"
#include "SomeRenderEngine.h"
#include "SomeRenderer.h"
#include "SomeTexture.h"
#include "StateCore.h"
#include "Timer.h"
#include "UIControl2D.h"
#include "UnorderedSet.h"
#include "View2D.h"
#include "WorldComponent.h"
#include "WorldNode.h"

namespace PJ {
    // EXTRA: boards, mine sweeper

    /// Responsible for loading scenes
    class SceneRepository {};

    /// Responsible for loading tile maps
    class TileMapRepository {};

    class PaintBrush {};

    class DelaunayBuilder {};

    // https://docs.unity3d.com/Packages/com.unity.postprocessing@3.0/manual/Quick-start.html
    // https://github.com/godotengine/godot/pull/80214
    class RenderPostProcessor {};

    class ShaderPostRenderOperation {};

    // TODO: don't name it effect? confusing with other "effects"
    class GrainPostRenderEffect {
        /// In order for this to work we need to always render into texture buffer
        /// Render texture buffer again into next texture buffer with effect shader
        /// Simple but needs some refactoring
    };

    class VignettePostRenderEffect {};

    class ColorGradePostRenderEffect {};
    class SomeCamera;

    // ? Isn't the camera a render system? If we have multiple cameras do we need multiple systems?
    class CameraRenderSystem {
        /// Cameras, in render order ?? Can each camera have its own shader/post effects?
        List<SomeCamera> cameras;
    };

    class CustomShader {};

    // This won't work. SwiftUI is Swift code, you'd need an entire Swift-language parser
    class SwiftUIRepository {};

    class MatrixBoardView2D : public WorldComponent<> {
    public:
        using Base = WorldComponent<>;
        enum class MoveResult {
            /// Can't move, it's animating
            Animating,

            /// Success
            Success,

            /// Destination is blocked
            Blocked,

            /// Error occurred
            Fail
        };

        Vector2Int matrixSize{ 5, 5 };

        //                MatrixBoard board;

        //                Node focusIndicator;

        //                std::optional<Vector2Int> mouseFocusedCell;
        //                MouseDevice mouseDevice = new();
        //
        //                override void Awake()
        //                {
        //                    Base::Awake();
        //
        //                    board = new MatrixBoard(matrixSize);
        //                }
        //
        //                override void Start()
        //                {
        //                    Base::Start();
        //
        //                    foreach (Transform childTransform in transform)
        //                    {
        //                        auto childNode =
        //                        childTransform.gameObject.GetComponent<MatrixNode2D>(); if
        //                        (nullptr
        //                        == childNode) { continue; }
        //
        //                        Put(childNode, childNode.initialOrigin);
        //                    }
        //                }
        //
        //                void RemoveAt(Vector2Int location, bool destroyGameObject = true)
        //                {
        //                    if (destroyGameObject)
        //                    {
        //                        auto gameObject = GameObjectAt(location);
        //                        if (gameObject)
        //                        {
        //                            Destroy(gameObject);
        //                        }
        //                    }
        //                    board.RemovePieceAt(location);
        //                }
        //
        //                void RemoveAll()
        //                {
        //                    for (int x = 0; x < matrixSize.x; x++)
        //                    {
        //                        for (int y = 0; y < matrixSize.y; y++)
        //                        {
        //                            RemoveAt(new Vector2Int(x, y));
        //                        }
        //                    }
        //                }
        //
        //                bool Replace(MatrixNode2D node, Vector2Int origin)
        //                {
        //                    auto pieceLocations = board.PieceLocationsAt(origin, node.piece);
        //                    if (nullptr == pieceLocations) { return false; }
        //
        //                    foreach (auto location in pieceLocations)
        //                    {
        //                        auto worldNode = WorldNodeAt(location);
        //                        if (worldNode)
        //                        {
        //                            Destroy(worldNode);
        //                        }
        //                        board.RemovePieceAt(location);
        //                    }
        //
        //                    return Put(node, origin);
        //                }
        //
        //                bool Put(MatrixNode2D node, Vector2Int origin)
        //                {
        //                    if (nullptr == node.piece)
        //                    {
        //                        PJLog("Error. Matrix node is missing a piece");
        //                        return false;
        //                    }
        //
        //                    auto putResult = board.PutPiece(node.piece, origin);
        //                    if (!putResult)
        //                    {
        //                        PJLog("Error. Can't place Matrix node at " +
        //                        origin.ToString());
        //                    }
        //
        //                    auto nodePosition = NodePosition(node);
        //                    node.transform.parent = transform;
        //                    node.transform.localPosition = nodePosition;
        //                    return putResult;
        //                }
        //
        //                override void OnUpdate(TimeSlice time)
        //                {
        //                    Base::OnUpdate(time);
        //
        //                    OnUpdateMouseInput();
        //                }
        //
        //                Vector2 CellSize() const {
        //                        return WorldSize / (new Vector2(matrixSize.x, matrixSize.y));
        //                }
        //
        //                std::optional<Vector2Int> LocationAtWorldPosition(Vector2 worldPosition)
        //                {
        //                    auto topLeft = TopLeftWorldPosition(transform.position);
        //
        //                    // The Matrix works in right-hand coordinate space
        //                    auto viewPosition = new Vector2(worldPosition.x - topLeft.x,
        //                    Mathf.Abs(worldPosition.y - topLeft.y)); if
        //                    (!IsViewPositionInside(viewPosition)) { return nullptr; }
        //
        //                    auto cell = LocationAtViewPosition(viewPosition);
        //                    return new std::optional<Vector2Int>(cell);
        //                }
        //
        //                std::optional<Vector2Int> LocationAtLocalPosition(Vector2 localPosition)
        //                {
        //                    return
        //                    LocationAtWorldPosition(transform.TransformPoint(localPosition));
        //                }
        //
        //                Vector2Int LocationAtViewPosition(Vector2 viewPosition)
        //                {
        //                    auto column = (int)(viewPosition.x / CellSize.x);
        //                    auto row = (int)(viewPosition.y / CellSize.y);
        //
        //                    return new Vector2Int(column, row);
        //                }
        //
        //                Vector2 LocationToLocalPosition(Vector2Int cell)
        //                {
        //                    auto x = -(WorldSize.x / 2.0f) + CellSize.x / 2.0f + cell.x *
        //                    CellSize.x; auto y = (WorldSize.y / 2.0f) * Vector2.Up.y + (CellSize.y
        //                    / 2.0f + cell.y * CellSize.y) * Vector2.Down.y;
        //
        //                    return new Vector2(x, y);
        //                }
        //
        //                Vector3 LocationToWorldPosition(Vector2Int cell)
        //                {
        //                    auto topLeft = TopLeftWorldPosition(transform.position);
        //                    auto x = topLeft.x + CellSize.x / 2.0f + cell.x * CellSize.x;
        //                    auto y = topLeft.y + (CellSize.y / 2.0f + cell.y * CellSize.y) *
        //                    Vector2.Down.y;
        //
        //                    return new Vector3(x, y, 0);
        //                }
        //
        //                MoveResult MovePiece(MatrixPiece piece, MapDirection direction, float
        //                duration)
        //                {
        //                    if (nullptr == piece) { return MoveResult.Fail; }
        //
        //                    auto oldOrigin = piece.origin;
        //                    auto newOrigin = oldOrigin + direction.Offset();
        //
        //                    return MovePiece(piece, newOrigin, duration);
        //                }
        //
        //                virtual MoveResult MovePiece(MatrixPiece piece, Vector2Int newOrigin,
        //                float duration)
        //                {
        //                    if (nullptr == piece) { return MoveResult.Fail; }
        //
        //                    if (nullptr == piece.owner || !piece.owner.TryGetTarget(out object
        //                    pieceOwner))
        //                    {
        //                        PJLog("Error. Piece owner is missing");
        //                        return MoveResult.Fail;
        //                    }
        //
        //                    MatrixNode2D node = pieceOwner as MatrixNode2D;
        //                    if (nullptr == node)
        //                    {
        //                        PJLog("Error. Need MatrixNode2D for Matrix board");
        //                        return MoveResult.Fail;
        //                    }
        //
        //                    if (node.IsAnimating) { return MoveResult.Animating; }
        //
        //                    auto oldOrigin = piece.origin;
        //                    if (newOrigin == oldOrigin) { return MoveResult.Fail; }
        //
        //                    auto excludeList = new HashSet<MatrixPiece>();
        //                    excludeList.Add(piece);
        //                    if (board.IsPieceBlockedAt(newOrigin, piece, excludeList))
        //                    {
        //                        return MoveResult.Blocked;
        //                    }
        //
        //                    board.RemovePiece(piece);
        //                    board.PutPiece(piece, newOrigin);
        //
        //                    auto endPosition = NodePosition(node);
        //
        //                    if (duration > 0)
        //                    {
        //                        node.moveAnimator = new(
        //                            new Vector2Interpolator(node.transform.localPosition,
        //                            endPosition), new(duration, AnimationCycleType.Once), new
        //                            SetBinding<Vector2>((Vector2 position) =>
        //                            node.transform.localPosition = position)
        //                        );
        //                        node.updatables.Add(node.moveAnimator);
        //                    }
        //                    else
        //                    {
        //                        node.transform.localPosition = endPosition;
        //                    }
        //
        //                    return MoveResult.Success;
        //                }
        //
        //                Vector2 NodePosition(MatrixNode2D node)
        //                {
        //                    auto pieceOrigin = node.piece.origin;
        //                    auto pieceSize = node.piece.Size;
        //
        //                    auto topLeftCellPosition = LocationToLocalPosition(pieceOrigin);
        //                    auto bottomRightCellPosition = LocationToLocalPosition(pieceOrigin +
        //                    new Vector2Int(pieceSize.x - 1, pieceSize.y - 1));
        //
        //                    auto x = topLeftCellPosition.x + (bottomRightCellPosition.x -
        //                    topLeftCellPosition.x) / 2.0f; auto y = topLeftCellPosition.y +
        //                    Mathf.Abs((bottomRightCellPosition.y - topLeftCellPosition.y) / 2.0f)
        //                    * Vector2.Down.y;
        //
        //                    return new Vector2(x, y);
        //                }
        //
        //                Node GameObjectAt(Vector2Int cell)
        //                {
        //                    return WorldNodeAt(cell);
        //                }
        //
        //                Node WorldNodeAt(Vector2Int cell)
        //                {
        //                    auto piece = board.PieceAt(cell);
        //                    if (nullptr == piece) { return nullptr; }
        //
        //                    auto cellOwner = piece.owner;
        //                    if (cellOwner.TryGetTarget(out object cellObject))
        //                    {
        //                        auto matrixNode = cellObject as MatrixNode2D;
        //                        if (matrixNode)
        //                        {
        //                            return matrixNode.gameObject;
        //                        }
        //                    }
        //
        //                    return nullptr;
        //                }
        //
        //        void OnUpdateMouseInput()
        //                {
        //                    if (!mouseDevice.IsAvailable()) { return; }
        //
        //                    auto worldPosition =
        //                    Camera.main.ScreenToWorldPoint(mouseDevice.ScreenPosition); if
        //                    (nullptr
        //                    == worldPosition) { return; }
        //
        //                    mouseFocusedCell = LocationAtWorldPosition(new
        //                    Vector2(worldPosition.x, worldPosition.y));
        //
        //                    if (focusIndicator)
        //                    {
        //                        if (nullptr != mouseFocusedCell)
        //                        {
        //                            auto cellPosition =
        //                            LocationToWorldPosition(mouseFocusedCell.value);
        //
        //                            focusIndicator.transform.position = new
        //                            Vector3(cellPosition.x, cellPosition.y,
        //                            focusIndicator.transform.position.Z); auto meshRenderer =
        //                            focusIndicator.GetComponent<MeshRenderer>(); if (meshRenderer)
        //                            {
        //                                meshRenderer.enabled = true;
        //                            }
        //                        }
        //                        else
        //                        {
        //                            auto meshRenderer =
        //                            focusIndicator.GetComponent<MeshRenderer>(); if (meshRenderer)
        //                            {
        //                                meshRenderer.enabled = false;
        //                            }
        //                        }
        //                    }
        //                    // PJLog(mouseFocusedCell.ToString());
        //                }
    };

    class MatrixNode2D : public WorldNode {};

    namespace ExampleMineSweeper {
        class Tile : public MatrixNode2D {
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
        class ExampleMineSweeperView : public MatrixBoardView2D {
        public:
            using Base = MatrixBoardView2D;

            enum class StateType { Running, GameOver };

            StateCore<StateType> sc;

            //     class BombCountComparer : IComparer<Vector2Int>
            //     {
            //         EMSBoardView boardView;

            //         BombCountComparer(EMSBoardView boardView)
            //         {
            //             this.boardView = boardView;
            //         }

            //         int Compare(Vector2Int lhs, Vector2Int rhs)
            //         {
            //             return boardView.BombSurroundCountAt(lhs, false) <
            //             boardView.BombSurroundCountAt(rhs, false) ? -1 : 1;
            //         }
            //     }

            //     SceneObjectRegistry sceneObjectRegistry;

            //     override void Awake()
            //     {
            //         Base::Awake();

            //         sceneObjectRegistry = FindObjectOfType<SceneObjectRegistry>();
            //     }

            void Start() override {
                Base::Start();
                PopulateBoard();
            }

            void PopulateBoard() {
                //             auto cellCount = matrixSize.x * matrixSize.y;
                //             auto bombPercentage = 0.15f;
                //             auto bombCount = bombPercentage * cellCount;
                //
                //             VectorList<Vector2Int> emptyCells = new();
                //
                //             for (int x = 0; x < matrixSize.x; x++)
                //             {
                //                 for (int y = 0; y < matrixSize.y; y++)
                //                 {
                //                     emptyCells.Add(Vector2Int(x, y));
                //                 }
                //             }
                //
                //             for (int i = 0; i < bombCount; i++)
                //             {
                //                 if (emptyCells.size() == 0)
                //                 {
                //                     break;
                //                 }
                //                 auto choiceIndex = Random.Range(0, emptyCells.size());
                //
                //                 auto choice = emptyCells[choiceIndex];
                //                 emptyCells.RemoveAt(choiceIndex);
                //
                //                 auto bombTileObject =
                //                 sceneObjectRegistry.InstantiateGameObject("tile", Vector3.Zero,
                //                 Quaternion.identity); auto bombTile =
                //                 bombTileObject.GetComponent<EMSTile>(); bombTile.type =
                //                 EMSTile.TileType::Bomb; Put(bombTile, choice);
                //             }
                //
                //             foreach (auto cell in emptyCells)
                //             {
                //                 auto tileObject =
                //                 sceneObjectRegistry.InstantiateGameObject("tile", Vector3.Zero,
                //                 Quaternion.identity); auto tile =
                //                 tileObject.GetComponent<EMSTile>(); Put(tile, cell);
                //             }
                //
                //             // Give the player a hint by clearing something for them
                //             emptyCells.Sort(new BombCountComparer(this));
                //             ClearTilesAt(emptyCells[0]);
            }

            //     override void OnPointerClickEvent(PointerClickUIEvent<LocalPosition> _event)
            //     {
            //         auto localPressPosition = _event.pressPosition;
            //         auto cellHit = LocationAtLocalPosition(localPressPosition.Position);

            //         if (cellHit.value == nullptr) { return; }

            //         auto cellGameObject = WorldNodeAt(cellHit.value);
            //         auto hitTile = cellGameObject.GetComponent<EMSTile>();

            //         switch (_event.Button)
            //         {
            //             case PointerInputButton.Left:
            //                 {
            //                     switch (core.State)
            //                     {
            //                         case StateType::Running:
            //                             if (hitTile.type == EMSTile.TileType::Bomb)
            //                             {
            //                                 RevealBombs();
            //                             }
            //                             else
            //                             {
            //                                 ClearTilesAt(cellHit.value);
            //                             }
            //                             break;
            //                         case StateType::GameOver:
            //                             Restart();
            //                             break;
            //                     }
            //                     break;
            //                 }
            //             case PointerInputButton.Right:
            //                 {
            //                     switch (hitTile.type)
            //                     {
            //                         case EMSTile.TileType::Tile:
            //                             {
            //                                 auto flagTileObject =
            //                                 sceneObjectRegistry.InstantiateGameObject("tile-flag",
            //                                 Vector3.Zero, Quaternion.identity); auto flagTile =
            //                                 flagTileObject.GetComponent<EMSTile>();
            //                                 Replace(flagTile, cellHit.value); break;
            //                             }
            //                         case EMSTile.TileType::Bomb:
            //                             {
            //                                 auto flagTileObject =
            //                                 sceneObjectRegistry.InstantiateGameObject("tile-flag",
            //                                 Vector3.Zero, Quaternion.identity); auto flagTile =
            //                                 flagTileObject.GetComponent<EMSTile>(); flagTile.type
            //                                 = EMSTile.TileType::BombFlag; Replace(flagTile,
            //                                 cellHit.value); break;
            //                             }
            //                         case EMSTile.TileType::ClearFlag:
            //                             {
            //                                 auto tileObject =
            //                                 sceneObjectRegistry.InstantiateGameObject("tile",
            //                                 Vector3.Zero, Quaternion.identity); auto tile =
            //                                 tileObject.GetComponent<EMSTile>(); Replace(tile,
            //                                 cellHit.value); break;
            //                             }
            //                         case EMSTile.TileType::BombFlag:
            //                             {
            //                                 auto tileObject =
            //                                 sceneObjectRegistry.InstantiateGameObject("tile",
            //                                 Vector3.Zero, Quaternion.identity); auto tile =
            //                                 tileObject.GetComponent<EMSTile>(); tile.type =
            //                                 EMSTile.TileType::Bomb; Replace(tile, cellHit.value);
            //                                 break;
            //                             }
            //                         default:
            //                             break;
            //                     }
            //                 }
            //                 break;
            //         }
            //     }

            void RemoveAll() {
                // TODO:
            }

            void Restart() {
                RemoveAll();
                PopulateBoard();
                sc.sm.SetState(StateType::Running);
            }

            void RevealBombs() {
                sc.sm.SetState(StateType::GameOver);

                //            for (int x = 0; x < matrixSize.x; x++)
                //            {
                //                for (int y = 0; y < matrixSize.y; y++)
                //                {
                //                    auto cell = Vector2Int(x, y);
                //                    auto gameObject = WorldNodeAt(cell);
                //                    if (gameObject)
                //                    {
                //                        auto tile = gameObject.GetComponent<EMSTile>();
                //
                //                        if (tile.IsBomb)
                //                        {
                //                            auto bombTileObject =
                //                            sceneObjectRegistry.InstantiateGameObject("tile-bomb",
                //                            Vector3.Zero, Quaternion.identity); auto bombTile =
                //                            bombTileObject.GetComponent<EMSTile>();
                //                            Replace(bombTile, cell);
                //                        }
                //                    }
                //                }
                //            }
            }

            void ClearTilesAt(Vector2Int cell) {
                ClearUnclearedTileAt(cell);
            }

            void ClearUnclearedTileAt(Vector2Int cell) {
                //                 auto gameObject = WorldNodeAt(cell);
                //                 if (nullptr == gameObject) { return; }
                //
                //                 auto tile = gameObject.GetComponent<EMSTile>();
                //                 if (tile.type == EMSTile.TileType::Tile)
                //                 {
                //                     auto clearTileObject =
                //                     sceneObjectRegistry.InstantiateGameObject("tile-clear",
                //                     Vector3.Zero, Quaternion.identity); auto clearTile =
                //                     clearTileObject.GetComponent<EMSClearTile>();
                //                     Replace(clearTile, cell);
                //
                //                     auto bombSurroundCount = BombSurroundCountAt(cell, false);
                //                     auto tmp = clearTile.text.GetComponent<TextMeshPro>();
                //
                //                     Color color = Color.red;
                //                     Color.RGBToHSV(color, out float h, out float s, out float v);
                //                     h = 1.0f - (Mathf.Min(4.0f, (float)bombSurroundCount)
                //                     / 4.0f);
                //
                //                     tmp.color = Color.HSVToRGB(h, s, v);
                //
                //                     switch (bombSurroundCount)
                //                     {
                //                         case 0:
                //                             Destroy(clearTile.text);
                //                             clearTile.text = nullptr;
                //                             break;
                //                         default:
                //                             tmp.text = bombSurroundCount.ToString();
                //                             break;
                //                     }
                //
                //                     if (BombSurroundCountAt(cell, true) == 0)
                //                     {
                //                         ClearUnclearedTileAt(Vector2Int(cell.x - 1, cell.y));
                //                         ClearUnclearedTileAt(Vector2Int(cell.x + 1, cell.y));
                //                         ClearUnclearedTileAt(Vector2Int(cell.x, cell.y - 1));
                //                         ClearUnclearedTileAt(Vector2Int(cell.x, cell.y + 1));
                //                     }
                //                 }
            }

            int BombSurroundCountAt(Vector2Int cell, bool skipDiagonals) {
                auto result = 0;

                //            for (int x = cell.x - 1; x <= cell.x + 1; x++)
                //            {
                //                for (int y = cell.y - 1; y <= cell.y + 1; y++)
                //                {
                //                    if (x == cell.x && y == cell.y) { continue; }
                //                    if (skipDiagonals)
                //                    {
                //                        if (x != cell.x && y != cell.y)
                //                        {
                //                            continue;
                //                        }
                //                    }
                //
                //                    auto tileObject = WorldNodeAt(Vector2Int(x, y));
                //                    if (nullptr == tileObject) { continue; }
                //
                //                    auto tile = tileObject.GetComponent<EMSTile>();
                //                    if (tile.IsBomb)
                //                    {
                //                        result++;
                //                    }
                //                }
                //            }

                return result;
            }
        };
    } // namespace ExampleMineSweeper

    namespace CheckersExample {
        class ExampleScene : Scene {
            void LoadInto(WorldNode& root) override {}
        };
    } // namespace CheckersExample

    namespace TopDown2DExample {
        class TopDown2DExampleScene : Scene {
            void LoadInto(WorldNode& root) override {}
        };
    } // namespace TopDown2DExample

    namespace Physics2DExample {
        class Physics2DExampleScene : Scene {
            // Tap in scene, produce random physics object
            void LoadInto(WorldNode& root) override {}
        };
    } // namespace Physics2DExample

    //    template <class Type>
    //    class FastBFSTreeNode {
    //    public:
    //        std::optional<Type> value;
    //        size_t depth = 0;
    //        size_t childCount = 0;
    //        size_t reservedChildCount = 0;
    //        size_t nextSiblingIndex = 0;
    //
    //        List<FastTreeNode<Type>> children;
    //    };
    //
    //    /// A fast tree, divided into slices for easy BFS iteration
    //    /// Order of the children is not guaranteed, useful in situations where we need
    //    /// tree relationships, but we don't care about the order of evaluation
    //    template <class Type>
    //    class FastUnstableBFSTree {
    //        using Node = FastTreeNode<Type>;
    //
    //        List<VectorList<Node>> slices{ VectorList<Node>(), 1 };
    //
    //        size_t nextOpenIndex = 0;
    //
    //        void Add(Type value) {
    //            slices[0].push_back(value);
    //        }
    //
    //        void AddTo(Node parent, Type value) {
    //            slices[parent->depth + 1].insert();
    //            nextOpenIndex++;
    //        }
    //    };

    // Example: attach a stencil to a view
    class ComposableRenderModifier {};

    class ProAnimationStateMachine {};

    class ProTimeline {};

    /// SDF-signed fonts for accessibility. Problem: need fonts with rounded corners?
    class SDFTextRenderer {};

    class Box2DSystem {
        /// ? Map of UUID to object id? Does each object have its own id?
        /// Each iteration after box2D system updates, it kinetically moves all the nodes
        /// If someone moves the node manually then shrug?
        /// ?How does the node know to send its move event back up to the Box2D system?
    }; // TODO: use #ifdef to enable

    class TextureBuffer {};

    class BehaviorGraphNode {};

    class Evaluator {};

    class ConditionBehavior {};

    class RandomSelectorBehavior {};

    class SelectorBehavior {};

    class SequenceSelector {};

    class StateBehavior {};

    class TimedBehavior {};

    class TimedStateBehavior {};

    class ParticleEmitter {};

    class SpeechRunner {};

    class a2DFXSystem {};

    class ResourcePack {};

    class RenderSystem {};

    class TileMap {};

    /// Abstracts when and how to move an object (move_and_slide vs just move)
    // !! should just be a func, no need to have a class
    class SomeMover {};

    /// Steers the camera
    class SomeCameraSteering : WorldComponent<> {
    public:
        /// Focus point of the camera
        WP<WorldNode> target;
    };

    /// Steers the camera in 2D space
    class SomeCameraSteering2D : WorldComponent<> {
    public:
        /// Focus point of the camera
        WP<WorldNode> target;
    };

    class Glyph {
    public:
        String value;

        /// Offset into the character (example: j hangs behind character bounds)
        int originX;

        /// Y offset for placing the trimmed glyph bitmap
        int offsetY;

        /// Distance from glyph origin to next glyph
        int advanceX;

        SP<AtlasTexture> texture;
    };

    class TextureAtlas;

    /// A font resource is defined by a texture atlas and a glyph map
    class Font : public Base {
    public:
        SP<TextureAtlas> atlas;
        UnorderedMap<String, Glyph> glyphs;
    };

    class TextLayout {
        VectorList<Vector3> BuildLayout(String string) {
            VectorList<Vector3> result;
            return result;
        }
    };

    // TODO: re-evaluate using funcs
    class CenterJustifyTextLayout {
        // func with Vector2 size
    };

    class RightJustifyTextLayout {};

    class BezierCurveTextLayout {};

    class TextMeshBuilder : public SomeMeshBuilder {
    public:
        String text;
        SP<Font> font;

        Mesh BuildMesh() override {
            Mesh result;

            /// TODO: how do we add modifiers?
            QuadMeshBuilder qmb;

            return result;
        }
    };

    class TextRenderer : public SomeRenderer {
    protected:
        String text;
        SP<Font> font;
        Mesh mesh;

    public:
        /// Size for justification
        Vector2 textSize;

        bool isLayoutNeeded = false;

        using LayoutFunc = std::function<VectorList<Vector3>(Vector2)>;

        LayoutFunc layoutFunc;

        String Text() const {
            return text;
        }

        void SetText(String text) {
            GUARD(text != this->text)
            this->text = text;
            isLayoutNeeded = true;
        }

        void ApplyLayoutIfNeeded() {
            GUARD(isLayoutNeeded)
            GUARD(layoutFunc)

            // layoutFunc(...)
            // build mesh with all glyphs

            isLayoutNeeded = false;
        }

        // ?? how do we pass in the # of visible characters to this func?
        VectorList<RenderModel> MakeRenderModels(RenderIntoModel const& model) {
            VectorList<RenderModel> result;

            GUARDR(owner, result)

            ApplyLayoutIfNeeded();

            // Cache the individual glyph meshes so if we use text visibility animation
            // we don't have to rebuild them all
            //            if (nullptr == material || nullptr == material->shaderProgram) {
            //                PJLog("ERROR. Missing material.");
            //                return result;
            //            }
            //
            //            RenderModelBuilder builder;
            //            auto renderModel = builder.Build(
            //                *material->shaderProgram, mesh, *material, ModelMatrix(),
            //                owner->transform->WorldPosition().z
            //            );

            //            result.push_back(renderModel);
            return result;
        }
    };

    class Label : View2D {
    protected:
        TextRenderer renderer;

    public:
        void SetVisibleRatio(float value) {}

        void SetVisibleCharacterCount(int value) {}

        // TODO: need a text modifier?
        void SetIsUppercase(bool value) {}
    };

    class ProgressBar : View2D {};

    /// Menu button presents a popup menu when tapped
    class MenuButtonControl : public UIControl2D {
    public:
        using Base = UIControl2D;

        void Awake() override {
            Base::Awake();

            // Create popup menu control as child and hide it
        }
    };

    class PopupMenuView : public UIControl2D {};

    class TextInputControl : public UIControl2D {};

    class AlertView : public View2D {};

    /// ?? Isn't this just a ToggleButton
    class SwitchControl : public UIControl2D {};

    /// A page view shows a single page based on user selection
    class PageView : public View2D {
        // ?? Subscribe to this without creating reference cycle
        PublishedValue<String> selection;

        /// Each tab is mapped to a child node. When that tab is selected, the child node will be
        /// made visible and other child nodes will be hidden
        UnorderedMap<String, WP<WorldNode>> tabs;

        void OnSelectionChange(String selection) {
            auto i = tabs.find(selection);
            GUARD(i != tabs.end())

            GUARD(!i->second.expired())

            auto selectedNode = i->second.lock();

            for (auto& tab : tabs) {
                GUARD(!tab.second.expired());

                auto tabLock = tab.second.lock();
                tabLock->SetActive(tabLock == selectedNode);
            }
        }
    };

    /// Tab control presents tab to control a page view
    class TabControl : public View2D {
        // ?? How are the tabs given size and renderers?
        // ?? Naming: how to distinguish between an abstract control and something that has concrete
        // UI? DuckButton <- duck is the UI theme
        WP<PageView> pageView;

        // ?? std::function<SP<SomeRenderer>()> makeRendererFunc;
    };

    class FocusCoordinator {
        // TODO: how do we attach metadata to nodes, like left-neighbor? Can only UI controls be
        // focused? SEE: https://docs.godotengine.org/en/stable/tutorials/ui/gui_navigation.html
    };

    class Feed : public View2D {};

    // PlatformWindow and Window are not the same thing
    class Window : public View2D {};

    class WindowManager {};

    class SegmentButton : public UIControl2D {
        struct Segment {
            String id;
            String title;
        };

        // TODO: PublishedValue
        PublishedValue<String> selection;
    };

    namespace ThemeKey {
        static auto const ButtonBackgroundColor = "button.background.color";
    }

    /// Collection of shared UI properties and modifiers
    /// Example: button.background.color = "0x442314"
    class UITheme {
    public:
        Tags tags;

        using ModifyViewFunc = std::function<void(View2D&)>;

        /// Allows for advanced behavior, where each individual view can be modified
        /// based on the theme
        List<ModifyViewFunc> viewModifiers;
    };

    /// Whenever a child or deep child is added, applies the theme view modifiers ?? how would it
    /// know about deep children though?
    class ThemedView : public View2D {};

    class ProjectionCamera : public SomeCamera {};

    class MetalRenderEngine : public SomeRenderEngine {};

    //    namespace VaryVectorAngle2D {
    //        TransformFunc<Angle> Make(Angle varyAngle) {
    //            override Angle Transform(Angle value)
    //            {
    //                auto angle = new Angle(Vector2(value.x, value.y));
    //                auto newAngle = Angle::DegreesAngle(random.VaryFloat(angle.Degrees,
    //                                                                   varyAngle.Degrees)); auto
    //                                                                   newAngleVector =
    //                                                                   (Vector2)newAngle; auto
    //                                                                   result =
    //                newAngleVector.Normalized() * value.Length(); return new(result.x, result.y,
    //                                                                         0);
    //            }
    //        }
    //    }

    class SomeCannonEmitter : Emitter {};

    class CannonEmitsModel : Updatable {
        /// Id for placement in collection of emits models (Example: 1 emits model per weapon type)
        String id;

        /// Min time between emits (decrease for rapid fire)
        float limitTime = 0;

        //        StandardEmitsBuilderModel emitsBuilderModel;

        SP<RateLimiter::Type> limiter = RateLimiter::Make();

        float velocityMagnitude = 0;

        //        float VelocityMagnitude
        //        {
        //            set
        //            {
        //                this.velocityMagnitude = value;
        //                emitsModel.VelocityMagnitude = value;
        //            }
        //        }
        //
        //        bool Fire() => limiter.Fire();
        //
        //        void OnUpdate(TimeSlice time)
        //        {
        //            limiter.minDelta = limitTime;
        //            limiter.OnUpdate(time);
        //        }

        // FUTURE: Burst fire
    };

    class CannonEmitter2D : SomeCannonEmitter {
        Angle emitAngle = Angle::DegreesAngle(0);

        //        virtual List<SP<WorldNode>> Fire(Angle angle) {
        //            if (model.Fire())
        //            {
        //                this.emitAngle = angle;
        //                return Emit();
        //            }
        //
        //            return new();
        //        }
        //
        //        override Vector3 EmitVelocity(Emit emit) {
        //            // TBD: is there a better algorithm for this?
        //            auto emitVector2 = Vector2(emit.velocity.x, emit.velocity.y);
        //            auto emitAngle = new Angle(emitVector2);
        //            auto thisEmitAngle = this.emitAngle;
        //            auto resultAngle = Angle.DegreesAngle(thisEmitAngle.Degrees +
        //            emitAngle.Degrees); auto result = (Vector2)resultAngle; result =
        //            result.Normalized(); result *= emitVector2.Length(); return new(result.x,
        //            result.y, 0);
        //        }
    };
} // namespace PJ
