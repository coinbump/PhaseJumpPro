#pragma once

#include "Animator.h"
#include "MatrixBoard.h"
#include "MatrixPieceHandler.h"
#include "View2D.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/18/24
 */
namespace PJ {
    /// Positions child nodes to correspond to cells in a matrix board
    /// Each node must have a MatrixPieceHandler, which stores a single piece
    /// Optionally performs animations when moving a piece to a new position
    class MatrixBoardView : public View2D {
    public:
        using Base = View2D;
        using This = MatrixBoardView;
        using MakeAnimatorFunc = PJ::MakeAnimatorFunc<Vector3, WorldNode&>;

        /// Result returned when we try to move a piece in the board
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

        enum class DestroyType {
            /// Keep the corresponding node
            Keep,

            /// Destroy the corresponding node
            Destroy
        };

        /// Model for the matrix board
        MatrixBoard board;

        // FUTURE: add mouse support if needed
        // WP<WorldNode> focusIndicator;
        // std::optional<Vector2Int> mouseFocusedCell;

        MatrixBoardView(Vector2 worldSize, Vector2Int matrixSize);

        /// Put the piece handler's piece into the board and move the piece's node to the cell
        /// position
        bool Put(MatrixPieceHandler& handler, Vector2Int origin);

        /// Remove the piece at the specified location and optionally destroy the corresponding
        /// world node
        void RemoveAt(Vector2Int location, DestroyType destroyNode = DestroyType::Destroy);

        /// Remove all pieces and optionally destroy the corresponding world node
        void RemoveAll(DestroyType destroyNode = DestroyType::Destroy);

        /// Puts this piece on the board, deleting any existing pieces that are blocking
        /// Optionally destroy the corresponding nodes
        bool Replace(
            MatrixPieceHandler& handler, Vector2Int origin,
            DestroyType destroyNode = DestroyType::Destroy
        );

        /// Moves the piece from its current location to a new location in the specified direction
        MoveResult
        Move(SP<MatrixPiece> piece, MapDirection direction, MakeAnimatorFunc makeAnimatorFunc);

        /// Moves the piece from its current location to a new origin
        MoveResult
        Move(SP<MatrixPiece> piece, Vector2Int newOrigin, MakeAnimatorFunc makeAnimatorFunc);

        Vec2I BoardSize() const {
            return board.Size();
        }

        Vec2I MatrixSize() const {
            return BoardSize();
        }

        /// @return Returns the cell size
        Vector2 CellSize() const;

        /// @return Returns the node for the cell location, or throws an exception if the location
        /// is invalid
        WorldNode& NodeAt(Vector2Int loc);

        /// Converts a position in view reading coordinates to a matrix location
        Vector2Int ViewPositionToLocation(Vector2 viewPosition);

        /// @return Returns the local node position for the piece handler
        Vector3 NodeLocalPosition(MatrixPieceHandler const& handler);

        /// @return Returns the local position of a specified cell location
        Vector3 LocationToLocalPosition(Vector2Int loc);

        /// Converts a local position to a cell location
        std::optional<Vector2Int> LocalPositionToLocation(Vector2 localPosition);

        /// Converts a world position to a cell location
        std::optional<Vector2Int> WorldPositionToLocation(Vector2 worldPosition);

        /// Converts a cell location to a world position
        Vector3 LocationToWorldPosition(Vector2Int cell);

        template <class Operator>
        void ForEachPiece(Operator op) {
            // Avoid iterate mutation crash
            auto iterPieces = board.Pieces();
            for (auto& piece : iterPieces) {
                op(*piece);
            }
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "MatrixBoardView";
        }

    protected:
        void PutChildPieces();

        // MARK: SomeWorldComponent

        void Awake() override {
            Base::Awake();
            GUARD(owner)

            PutChildPieces();
        }

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override {
            Base::OnUpdate(time);

            // FUTURE: support mouse hover if needed
            // OnUpdateMouseInput();
        }
    };
} // namespace PJ
